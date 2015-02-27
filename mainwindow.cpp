#include <QUrl>
#include <QFile>
#include <QDir>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QTextStream>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "netrequestfactory.h"
#include "tweet.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    _netMngr = std::unique_ptr<QNetworkAccessManager>(new QNetworkAccessManager());

    connect(_netMngr.get(), SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    /*NetRequestFactory nrf;
    _netMngr->get(nrf.homeTimeline());*/

    testHTML();
}

MainWindow::~MainWindow()
{

}

void MainWindow::testHTML()
{
    QFile file("hometl.json");
    file.open(QIODevice::ReadOnly);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QString html = "";
    QTextStream out(&html, QIODevice::WriteOnly);

    out
    << "<!DOCTYPE html>"
    << "<html>"
    << "<head>"
    << "<link rel=\"stylesheet\" type=\"text/css\" href=\"" << "file:///" + QDir::currentPath() + "/twittsk.css" << "\">"
    << "</head>"
    << "<body>";

    for(const auto& v : jsonDoc.array()) {
        QJsonObject obj = v.toObject();
        Tweet tweet(obj);
        out << tweet.toHTML();
    }

    out << "</body></html>";


    _ui->webView->setHtml(html);
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    qDebug() << "replyFinished()";
    QByteArray all = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(all);

    QFile file("hometl.json");
    file.open(QIODevice::WriteOnly);
    file.write(jsonDoc.toJson());
    file.close();

    reply->disconnect();
    reply->deleteLater();
}
