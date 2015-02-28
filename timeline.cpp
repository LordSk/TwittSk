#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "timeline.h"

Timeline::Timeline(QObject *parent):
    QObject(parent)
{
    connect(&_netAM, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(replyFinished(QNetworkReply*)));
}

Timeline::~Timeline()
{

}

void Timeline::replyFinished(QNetworkReply *reply)
{

}



HomeTimeline::HomeTimeline(QObject *parent):
    Timeline(parent)
{

}

void HomeTimeline::update()
{
    _netAM.get(_netReqFact.homeTimeline({
        {"count", "5"},
        {"since_id", "571729026986979328"}
    }));
}

void HomeTimeline::replyFinished(QNetworkReply *reply)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());

    for(const auto& v : jsonDoc.array()) {
        QJsonObject obj = v.toObject();
        Tweet tweet(obj);
        _tweets.insert({tweet.id(), tweet});
        qDebug() << tweet.id();
    }

    reply->disconnect();
    reply->deleteLater();
}
