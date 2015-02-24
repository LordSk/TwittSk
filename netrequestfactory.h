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

    // Simple random class
    struct Random {
        std::mt19937 gen;
        int getInt(int min, int max);
    } _rand;

    /**
     * @brief Get unix timestamp
     * @return unix timestamp
     */
    int64_t getTimestamp() const;

    /**
     * @brief Generate a 32 char long random string
     * @return random string
     */
    QString genUniqueToken();

    /**
     * @brief Generate an authorization header
     * @param url base url
     * @return authorization header
     */
    QString genAuthHeader(const QUrl& url);

    /**
     * @brief Generate an OAuth signature
     * @param url base url
     * @param nonce unique token
     * @param timestamp unix timestamp
     * @return OAuth signature
     */
    QString genSignature(const QUrl& url, const QString &nonce, const QString &timestamp);

public:
    NetRequestFactory();

    /**
     * @brief Create a home timeline request
     * @return home timeline request
     */
    QNetworkRequest homeTimeline();
};

#endif // NETREQUESTFACTORY_H
