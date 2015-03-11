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

    virtual void fetchTop() = 0;
    virtual void fetchBottom(const QString& fromId) = 0;
    QString getHTML() const;
    void markAsRead();

protected:
    QNetworkAccessManager _netAM;
    NetRequestFactory _netReqFact;
    std::map<QString, Tweet, std::greater<QString>> _tweets;
    int _newTweetsCount;

signals:
    void topFetched(int newTweetsCount);
    void bottomFetched();

protected slots:
    virtual void replyFinished(QNetworkReply* reply);
};


class HomeTimeline: public Timeline
{
    Q_OBJECT

public:
    explicit HomeTimeline(QObject* parent = nullptr);
    void fetchTop();
    void fetchBottom(const QString& fromId);

protected slots:
    virtual void replyFinished(QNetworkReply* reply);
};

#endif // TIMELINE_H
