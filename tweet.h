#ifndef TWEET_H
#define TWEET_H

#include <QString>

class Tweet
{
    QString _idStr;
    QString _rawText;
    QString _htmlText;

    struct {
        QString avatarSrc;
        QString displayName;
        QString userName;
    } _poster;

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
