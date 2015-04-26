#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QDebug>
#include <QLocale>
#include <map>
#include "tweet.h"

Tweet::Tweet(const QJsonObject &obj)
{
    QLocale usLocale(QLocale::English, QLocale::UnitedStates);
    _date = usLocale.toDateTime(obj["created_at"].toString(), "ddd MMM dd hh:mm:ss +0000 yyyy");
    qDebug() << usLocale.toString(_date, "dd/MM/yyyy hh:mm:ss");

    _idStr = obj["id_str"].toString();
    _rawText = obj["text"].toString();
    _htmlText = _rawText;

    QJsonObject user = obj["user"].toObject();
    _poster.displayName = user["name"].toString();
    _poster.avatarSrc = user["profile_image_url"].toString();
    _poster.userName = user["screen_name"].toString();

    QJsonObject entities = obj["entities"].toObject();

    _retweeted = false;
    if(obj.contains("retweeted_status")) {
        QJsonObject rtObj = obj["retweeted_status"].toObject();
        entities = rtObj["entities"].toObject();
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

    << "<div class=\"userInfo\">"
        << "<div class=\"avatar\"><img src=\"" << _poster.avatarSrc << "\"></div>"
        << "<div class=\"name\">"
            << "<div class=\"displayName\"><a href=\"" << TWITTER_URL << "/" << _poster.userName
            << "\">" << _poster.displayName << "</a></div>"
            << "<div class=\"userName\"><a href=\"" << TWITTER_URL << "/" << _poster.userName
            << "\">@" << _poster.userName << "</a></div>"
        << "</div>";
    out << "</div>";

    if(_retweeted) {

    }

    out << "<div class=\"tweetContent\">"
        << "<div class=\"text\">" << _htmlText << "</div>";

        if(!_media.undefined) {
            out << "<div class=\"mediaPreview\">"
                        "<a href=\"" << _media.url << ":large\"><img src=\"" << _media.url << "\"></a>"
                   "</div>";
        }

    out << "</div>" // tweetContent

    << "</div>"; // tweetBody

    return html;
}

QString Tweet::id() const
{
    return _idStr;
}
