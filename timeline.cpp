#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include "timeline.h"

Timeline::Timeline(QObject *parent):
    QObject(parent),
    _newTweetsCount(0)
{
    connect(&_netAM, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(replyFinished(QNetworkReply*)));
}

QString Timeline::getHTML() const
{
    QString html = "";
    int max = 50;

    // open newTweets block
    if(_newTweetsCount > 0)
        html += "<div class=newTweets>";

    int ntc = _newTweetsCount;
    for(const auto& pair : _tweets) {
        if(max-- == 0)
            break;

        // close newTweets block
        if(ntc-- == 0 && _newTweetsCount > 0)
            html += "</div>";

        html += pair.second.toHTML();
    }

    return html;
}

void Timeline::markAsRead()
{
    _newTweetsCount = 0;
}

void Timeline::replyFinished(QNetworkReply *reply)
{
    reply->disconnect();
    reply->deleteLater();
}



HomeTimeline::HomeTimeline(QObject *parent):
    Timeline(parent)
{

}

void HomeTimeline::fetchTop()
{
    QString count = "100";

    if(_tweets.size() > 0) {
        _netAM.get(_netReqFact.homeTimeline({
            {"count", count},
            {"since_id", _tweets.begin()->first}
        }));
    }
    else {
        _netAM.get(_netReqFact.homeTimeline({
            {"count", count},
        }));
    }
}

void HomeTimeline::fetchBottom(const QString &fromId)
{
    /*QString count = "100";

    if(_tweets.size() > 0) {
        _netAM.get(_netReqFact.homeTimeline({
            {"count", count},
            {"since_id", _tweets.begin()->first}
        }));
    }
    else {
        _netAM.get(_netReqFact.homeTimeline({
            {"count", count},
        }));
    }*/
}

void HomeTimeline::replyFinished(QNetworkReply *reply)
{
    /*QFile file(QDir::currentPath() + "/raw.reply");
    file.open(QIODevice::WriteOnly);
    file.write(reply->readAll());
    file.close();*/

    /*QFile file(QDir::currentPath() + "/raw.reply");
    file.open(QIODevice::ReadOnly);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();*/

    // emoji parsing?
    /*QByteArray replyContent = reply->readAll();
    QString str = QString::fromUtf8(replyContent);
    QRegExp regex("([\\\\]{1}u.{4})");
    str.replace(regex, " ");*/

    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
    /*QFile file(QDir::currentPath() + "/hometl.json");
    file.open(QIODevice::WriteOnly);
    file.write(jsonDoc.toJson());
    file.close();*/

    /*QFile file(QDir::currentPath() + "/hometl.json");
    file.open(QIODevice::ReadOnly);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();*/

    int newTweetsCount = 0;

    for(const auto& v : jsonDoc.array()) {
        Tweet tweet(v.toObject());
        _tweets.insert({tweet.id(), tweet});
        //qDebug() << tweet.id();
        newTweetsCount++;
    }

    _newTweetsCount += newTweetsCount;

    emit topFetched(_newTweetsCount);

    Timeline::replyFinished(reply); // close reply
}
