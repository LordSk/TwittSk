#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "netrequestfactory.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    _netMngr = std::unique_ptr<QNetworkAccessManager>(new QNetworkAccessManager());

    connect(_netMngr.get(), SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    /*QNetworkRequest request;
    request.setUrl(QUrl("https://api.twitter.com/1.1/statuses/home_timeline.json"));
    request.setHeader(QNetworkRequest::UserAgentHeader, "TwittSk 0.1");
    request.setRawHeader("Authorization", "OAuth oauth_consumer_key=\"2o0bLmCaUOna60etgqD5RNJou\",\
         oauth_nonce=\"570ce51935684585090e4305068402f3\", oauth_signature=\"bq0nosQFbssjus6va5X1Mgj7xn0%3D\",\
         oauth_signature_method=\"HMAC-SHA1\", oauth_timestamp=\"1424769469\",\
         oauth_token=\"92971104-WqmfH0O9eUZthVI9LqcdACLC37ekfNzW8LoVhKoRM\", oauth_version=\"1.0\"");
    _netMngr->get(request);*/

    NetRequestFactory nrf("2o0bLmCaUOna60etgqD5RNJou", "92971104-WqmfH0O9eUZthVI9LqcdACLC37ekfNzW8LoVhKoRM");
}

MainWindow::~MainWindow()
{

}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    qDebug() << "replyFinished(): ";
    QByteArray all = reply->readAll();
    QString str = QString::fromLatin1(all);
    _ui->textEdit->setText(str);
    reply->disconnect();
    reply->deleteLater();
}
