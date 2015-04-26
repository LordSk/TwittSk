#ifndef TWEET_H
#define TWEET_H

#include <QString>
#include <QDateTime>

#define TWITTER_URL QString("https://twitter.com")

class Tweet
{
    QString _idStr;
    QDateTime _date;
    QString _rawText;
    QString _htmlText;

    struct UserInfo
    {
        QString avatarSrc;
        QString displayName;
        QString userName;
    };

    UserInfo _poster;
    UserInfo _rtUser;
    bool _retweeted;

    struct {
        QString url;
        QString type;
        bool undefined;
    } _media;

public:

    explicit Tweet(const QJsonObject& obj);

    QString toHTML() const;
    QString id() const;
};

#endif // TWEET_H
