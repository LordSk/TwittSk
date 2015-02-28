#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include "tweet.h"

Tweet::Tweet(const QJsonObject &obj)
{
    _idStr = obj["id_str"].toString();
    _text = obj["text"].toString();

    QJsonObject user = obj["user"].toObject();
    _poster.displayName = user["name"].toString();
    _poster.avatarSrc = user["profile_image_url"].toString();
    _poster.userName = user["screen_name"].toString();

    QJsonObject entities = obj["entities"].toObject();

    _media.undefined = true;
    if(!entities["media"].isUndefined()) {
        QJsonObject media = (*entities["media"].toArray().begin()).toObject();
        _media.url = media["media_url_https"].toString();
        _media.type = media["type"].toString();
        _media.undefined = false;
    }
}

QString Tweet::toHTML() const
{
    QString html = "";
    QTextStream out(&html, QIODevice::WriteOnly);

    out
    << "<div class=\"tweetBody\">"

    << "<div class=\"leftPanel\">"
    << "<div class=\"avatar\"><img src=\"" << _poster.avatarSrc << "\"></div>"
    << "</div>"

    << "<div class=\"rightPanel\">"
    << "<div class=\"posterName\">" << _poster.displayName << "</div>"
    << "<div class=\"text\">" << _text << "</div>";

    if(!_media.undefined) {
        out << "<div class=\"mediaPreview\"><img src=\"" << _media.url << "\"></div>";
    }

    out << "</div>" // rightPanel
    << "</div>"; // tweetBody

    return html;
}

QString Tweet::id() const
{
    return _idStr;
}
