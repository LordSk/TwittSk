#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QDebug>
#include <map>
#include "tweet.h"

Tweet::Tweet(const QJsonObject &obj)
{
    _idStr = obj["id_str"].toString();
    _rawText = obj["text"].toString();
    _htmlText = _rawText;

    QJsonObject user = obj["user"].toObject();
    _poster.displayName = user["name"].toString();
    _poster.avatarSrc = user["profile_image_url"].toString();
    _poster.userName = user["screen_name"].toString();

    QJsonObject entities = obj["entities"].toObject();

    if(obj.contains("retweeted_status")) {
        QJsonObject rtObj = obj["retweeted_status"].toObject();
        entities = rtObj["entities"].toObject();
        _rawText = rtObj["text"].toString();
    }

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
            QString link = "<a href=\"https://twitter.com/hashtag/" + htObj["text"].toString()
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
            QString link = "<a href=\"https://twitter.com/" + umObj["screen_name"].toString()
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
        << "<div class=\"posterName\">" << _poster.displayName << "</div>"
    << "</div>"

    << "<div class=\"tweetContent\">"
        << "<div class=\"text\">" << _htmlText << "</div>";

        if(!_media.undefined) {
            out << "<div class=\"mediaPreview\"><img src=\"" << _media.url << "\"></div>";
        }

    out << "</div>" // tweetContent

    << "</div>"; // tweetBody

    return html;
}

QString Tweet::id() const
{
    return _idStr;
}
