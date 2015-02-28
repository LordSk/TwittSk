#ifndef TIMELINE_H
#define TIMELINE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <map>
#include "netrequestfactory.h"
#include "tweet.h"

class Timeline : public QObject
{
    Q_OBJECT

public:
    explicit Timeline(QObject* parent = nullptr);
    virtual ~Timeline();

    virtual void update() = 0;

protected:
    QNetworkAccessManager _netAM;
    NetRequestFactory _netReqFact;
    std::map<QString, Tweet> _tweets;

private slots:
    virtual void replyFinished(QNetworkReply* reply);
};


class HomeTimeline: public Timeline
{
    Q_OBJECT

public:
    explicit HomeTimeline(QObject* parent = nullptr);
    void update();

private slots:
    virtual void replyFinished(QNetworkReply* reply);
};

#endif // TIMELINE_H
