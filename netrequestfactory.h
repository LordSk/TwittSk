#ifndef NETREQUESTFACTORY_H
#define NETREQUESTFACTORY_H

#include <QString>
#include <QNetworkRequest>
#include <random>

class NetRequestFactory
{
    #define SIGNATURE_METHOD "HMAC-SHA1"
    #define OAUTH_VERSION "1.0"
    const QString _consumerKey;
    const QString _token;
    const QString _consumerKeySecret;
    const QString _tokenSecret;

    struct Random {
        std::mt19937 gen;
        int getInt(int min, int max);
    } _rand;

    int64_t getTimestamp() const;
    QString genUniqueToken();
    QString genAuthHeader(const QUrl& url);
    QString genSignature(const QUrl& url, const QString &nonce, const QString &timestamp);

public:
    NetRequestFactory();

    QNetworkRequest homeTimeline();
};

#endif // NETREQUESTFACTORY_H
