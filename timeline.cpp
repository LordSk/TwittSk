#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include "timeline.h"

Timeline::Timeline(QObject *parent):
    QObject(parent)
{
    connect(&_netAM, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

const TweetMap &Timeline::getTweets() const
{
    return _tweets;
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
#ifdef NDEBUG
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
#else
    QFile file(QDir::currentPath() + "/hometl.json");
    file.open(QIODevice::ReadOnly);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();

    int newTweetsCount = 0;

    for(const auto& v : jsonDoc.array()) {
        Tweet tweet(v.toObject());
        _tweets.insert({tweet.id(), tweet});
        //qDebug() << tweet.id();
        newTweetsCount++;
    }

    emit topFetched(newTweetsCount);
#endif
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
    QByteArray replyStr = file.readAll();
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

    emit topFetched(newTweetsCount);

    Timeline::replyFinished(reply); // close reply
}
