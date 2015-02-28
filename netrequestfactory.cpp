#include <QDebug>
#include <QUrl>
#include <QMessageAuthenticationCode>
#include <chrono>
#include <map>
#include "netrequestfactory.h"
#include "secret_keys.h"

int64_t NetRequestFactory::getTimestamp() const
{
    return std::chrono::seconds(std::time(NULL)).count();
}

QString NetRequestFactory::genUniqueToken()
{
    QString array;
    array.resize(32);

    for(int i = 0; i < 32; i++) {
        int c = _rand.getInt(65, 122);
        if(c >= 91 && c <= 96) // special symbol, replace
            c = _rand.getInt(48, 57); // [0-9]

        array[i] = (char)c;
    }

    return array;
}

QString NetRequestFactory::genAuthHeader(const QUrl &baseUrl, const std::map<QString, QString>& urlParams)
{
    QString uniToken = genUniqueToken();
    QString timestamp = QString::number(getTimestamp());
    QString signature = genSignature(baseUrl, uniToken, timestamp, urlParams);

    QString str = "OAuth ";
    str += "oauth_consumer_key=\"" + QUrl::toPercentEncoding(_consumerKey) + "\", ";
    str += "oauth_nonce=\"" + QUrl::toPercentEncoding(uniToken) + "\", ";
    str += "oauth_signature=\"" + QUrl::toPercentEncoding(signature) + "\", ";
    str += "oauth_signature_method=\"" + QUrl::toPercentEncoding((SIGNATURE_METHOD)) + "\", ";
    str += "oauth_timestamp=\"" + QUrl::toPercentEncoding(timestamp) + "\", ";
    str += "oauth_token=\"" + QUrl::toPercentEncoding(_token) + "\", ";
    str += "oauth_version=\"" + QUrl::toPercentEncoding(QString(OAUTH_VERSION)) + "\"";
    return str;
}

QString NetRequestFactory::genSignature(const QUrl &baseUrl, const QString& nonce, const QString& timestamp,
                                        const std::map<QString, QString>& urlParams)
{
    // Collect parameters
    std::map<QString, QString> params;
    // add url parameters (?param=something&....)
    params.insert(urlParams.begin(), urlParams.end());
    params["oauth_consumer_key"] = _consumerKey;
    params["oauth_nonce"] = nonce;
    params["oauth_signature_method"] = QString(SIGNATURE_METHOD);
    params["oauth_timestamp"] = timestamp;
    params["oauth_token"] = _token;
    params["oauth_version"] = QString(OAUTH_VERSION);

    QString paramStr("");

    for(const auto p : params) {
        paramStr += QUrl::toPercentEncoding(p.first);
        paramStr += "=";
        paramStr += QUrl::toPercentEncoding(p.second);
        paramStr += "&";
    }

    paramStr = paramStr.left(paramStr.length()-1); // remove last "&" -> not very clean
    //qDebug() << paramStr;

    // Create the signature base string
    QString baseStr = "GET&";
    baseStr += QUrl::toPercentEncoding(baseUrl.toString());
    baseStr += "&";
    baseStr += QUrl::toPercentEncoding(paramStr);
    //qDebug() << baseStr;

    // Signing key
    QString signingKey = QUrl::toPercentEncoding(_consumerKeySecret);
    signingKey += "&";
    signingKey += QUrl::toPercentEncoding(_tokenSecret);
    //qDebug() << signingKey;

    QString signature = QMessageAuthenticationCode::hash(baseStr.toLatin1(), signingKey.toLatin1(), QCryptographicHash::Sha1).toBase64();
    //qDebug() << signature;

    return signature;
}

NetRequestFactory::NetRequestFactory()
    : _consumerKey(CONSUMER_KEY),
      _token(OAUTH_TOKEN),
      _consumerKeySecret(CONSUMER_KEY_SECRET),
      _tokenSecret(OAUTH_TOKEN_SECRET)
{
    _rand.gen.seed(getTimestamp());
}

QNetworkRequest NetRequestFactory::homeTimeline(const std::map<QString, QString>& params)
{
    QString baseUrlStr = "https://api.twitter.com/1.1/statuses/home_timeline.json";
    QString urlStr = baseUrlStr;
    if(params.size() > 0) {
        urlStr += "?";

        unsigned int i = 0;
        for(const auto& pair: params) {
            urlStr += pair.first + "=" + pair.second;
            if(i < params.size()-1)
               urlStr += "&";
            i++;
        }
    }

    //qDebug() << urlStr;

    QNetworkRequest request;
    request.setUrl(QUrl(urlStr));
    request.setHeader(QNetworkRequest::UserAgentHeader, "TwittSk 0.1");
    request.setRawHeader("Authorization", genAuthHeader(QUrl(baseUrlStr), params).toLatin1());
    return request;
}


int NetRequestFactory::Random::getInt(int min, int max)
{
    std::uniform_int_distribution<> uniInt(min, max);
    return uniInt(gen);
}
