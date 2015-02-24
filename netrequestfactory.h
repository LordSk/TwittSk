#ifndef NETREQUESTFACTORY_H
#define NETREQUESTFACTORY_H

#include <QString>
#include <random>

class NetRequestFactory
{
    #define SIGNATURE_METHOD "HMAC-SHA1"
    #define OAUTH_VERSION "1.0"
    QString _consumerKey;
    QString _token;

    struct Random {
        std::mt19937 gen;
        int getInt(int min, int max);
    } _rand;

    int64_t getTimestamp() const;
    QString genUniqueToken();

public:
    NetRequestFactory(const QString& consumerKey, const QString& token);
};

#endif // NETREQUESTFACTORY_H
