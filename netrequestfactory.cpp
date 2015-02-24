#include <QDebug>
#include <chrono>
#include "netrequestfactory.h"

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

NetRequestFactory::NetRequestFactory(const QString &consumerKey, const QString &token)
    : _consumerKey(consumerKey),
      _token(token)
{
    _rand.gen.seed(getTimestamp());

    for(int i = 0; i < 24; i++)
        qDebug() << genUniqueToken();
}


int NetRequestFactory::Random::getInt(int min, int max)
{
    std::uniform_int_distribution<> uniInt(min, max);
    return uniInt(gen);
}
