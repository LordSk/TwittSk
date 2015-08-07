#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QDebug>
#include <QLocale>
#include <QTimeZone>
#include <map>
#include "tweet.h"

Tweet::Tweet(const QJsonObject &obj)
{
    // parse tweet datetime
    _usLoc = QLocale(QLocale::English, QLocale::UnitedStates);
    _date = _usLoc.toDateTime(obj["created_at"].toString(), "ddd MMM dd hh:mm:ss +0000 yyyy");
    _date.setTimeSpec(Qt::UTC);
    _date = _date.toLocalTime(); // convert to local time

    _idStr = obj["id_str"].toString();
    _rawText = obj["text"].toString();
    _htmlText = _rawText;

    QJsonObject user = obj["user"].toObject();
    _poster.displayName = user["name"].toString();
    _poster.avatarSrc = user["profile_image_url"].toString();
    _poster.userName = user["screen_name"].toString();

    QJsonObject entities = obj["entities"].toObject();
    QJsonObject extendedEnt = obj["extended_entities"].toObject();

    _retweeted = false;
    if(obj.contains("retweeted_status")) {
        QJsonObject rtObj = obj["retweeted_status"].toObject();
        entities = rtObj["entities"].toObject();
        extendedEnt = rtObj["extended_entities"].toObject();
        _rawText = rtObj["text"].toString();
        _retweeted = true;

        QJsonObject rtUser = rtObj["user"].toObject();
        _rtUser.displayName = rtUser["name"].toString();
        _rtUser.avatarSrc = rtUser["profile_image_url"].toString();
        _rtUser.userName = rtUser["screen_name"].toString();
    }



    // parse entities using twitter's indice system (emojis break this)
    std::map<int, QString> strMap;
    int i = 0;
    for(const auto& c : _rawText) {
        strMap.insert({i++, QString(c)});
    }

        // hashtag
    if(entities.contains("hashtags")) {
        for(const auto& v : entities["hashtags"].toArray()) {
            QJsonObject htObj = v.toObject();
            QJsonArray indices = htObj["indices"].toArray();
            int start = indices[0].toInt();
            QString link = "<a href=\"" + TWITTER_URL + "/hashtag/" + htObj["text"].toString()
                    + "\">#" + htObj["text"].toString() + "</a>";
            strMap[start] = link;
            for(int i = start + 1; i < indices[1].toInt(); i++) {
                strMap.erase(i);
            }
        }
    }

        // media
    _media.undefined = true;
    if(entities.contains("media")) {
        QJsonObject media = entities["media"].toArray()[0].toObject();
        _media.url = media["media_url_https"].toString();
        _media.type = media["type"].toString();
        _media.undefined = false;

        QJsonArray indices = media["indices"].toArray();
        for(int i = indices[0].toInt(); i < indices[1].toInt(); i++) {
            strMap.erase(i);
        }
    }

    if(extendedEnt.contains("media")) {
        QJsonObject media = extendedEnt["media"].toArray()[0].toObject();
        _media.undefined = false;
        _media.url = media["media_url_https"].toString();
        _media.type = media["type"].toString();

        if(_media.type == "video") {
            QJsonObject videoInfo = media["video_info"].toObject();
            QJsonArray variants = videoInfo["variants"].toArray();

            for(const auto& v : variants) {
                QJsonObject vObj = v.toObject();
                if(vObj["content_type"].toString() == "video/mp4") {
                    _media.url = vObj["url"].toString();
                    _media.videoType = "video/mp4";
                }
            }
        }

        QJsonArray indices = media["indices"].toArray();
        for(int i = indices[0].toInt(); i < indices[1].toInt(); i++) {
            strMap.erase(i);
        }
    }

        // urls
    for(const auto &v : entities["urls"].toArray()) {
        QJsonObject urlObj = v.toObject();
        QJsonArray indices = urlObj["indices"].toArray();
        int start = indices[0].toInt();
        QString link = "<a href=\"" + urlObj["expanded_url"].toString()
                + "\">" + urlObj["display_url"].toString() + "</a>";
        strMap[start] = link;
        for(int i = start + 1; i < indices[1].toInt(); i++) {
            strMap.erase(i);
        }
    }

        // user_mentions
    if(entities.contains("user_mentions")) {
        for(const auto &v : entities["user_mentions"].toArray()) {
            QJsonObject umObj = v.toObject();
            QJsonArray indices = umObj["indices"].toArray();
            int start = indices[0].toInt();
            QString link = "<a href=\"" + TWITTER_URL + "/" + umObj["screen_name"].toString()
                    + "\">@" + umObj["screen_name"].toString() + "</a>";
            strMap[start] = link;
            for(int i = start + 1; i < indices[1].toInt(); i++) {
                strMap.erase(i);
            }
        }
    }

    _htmlText = "";
    for(const auto& pair : strMap) {
        _htmlText += pair.second;
    }
}

QString Tweet::toHTML() const
{
    QString html = "";
    QTextStream out(&html, QIODevice::WriteOnly);

    out
    << "<div class=\"tweetBody\">"
    << "<div class=\"tweetHeader\">"

    // username, avatar
    << "<div class=\"userInfo\">"
        << "<div class=\"avatar\"><img src=\"" << _poster.avatarSrc << "\"></div>"
        << "<div class=\"name\">"
            << "<div class=\"displayName\"><a href=\"" << TWITTER_URL << "/" << _poster.userName
            << "\">" << _poster.displayName << "</a></div>"
            << "<div class=\"userName\"><a href=\"" << TWITTER_URL << "/" << _poster.userName
            << "\">@" << _poster.userName << "</a></div>"
        << "</div>";
    out << "</div>";

    // date
    out << "<div class=\"date\">";

    QDateTime curDate = QDateTime::currentDateTime();
    if(_date.daysTo(curDate) < 1) {
        int secOffset = _date.secsTo(curDate);
        float hours = (float)secOffset / 3600.f;
        float min  = (float)secOffset / 60.f;

        if(hours >= 1.f) {
            out << (int)hours << "h";
        }
        else if(min >= 1.f) {
            out << (int)min << " min";
        }
        else {
            out << secOffset << "s";
        }
    }
    else {
        out << _usLoc.toString(_date, "dd MMM");
    }

    out << "</div>";

    out << "</div>"; // tweetHeader

    out << "<div class=\"tweetContent\">"
        << "<div class=\"text\">" << _htmlText << "</div>";

        if(!_media.undefined) {
            out << "<div class=\"mediaPreview\">"
                        "<a href=\"" << _media.url << ":large\"><img src=\"" << _media.url << "\"></a>"
                   "</div>";
            if(_media.type == "video") {
                out << "<video width=\"320\" height=\"240\" controls>"
                       "<source src=\"" << _media.url << "\" type=\""<< _media.videoType << "\">"
                     "</video>";
            }
        }

    out << "</div>"; // tweetContent

    if(_retweeted) {
        out << "<div class=\"retweeted\">"
        << "<a href=\"" << TWITTER_URL << "/" << _rtUser.userName
        << "\">" << _rtUser.displayName << "</a></div>";
    }

    out << "</div>"; // tweetBody

    return html;
}

QString Tweet::id() const
{
    return _idStr;
}
