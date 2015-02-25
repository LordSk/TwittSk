#include <QUrl>
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

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

    QJsonObject obj = jsonDoc.array().at(0).toObject();
    qDebug() << obj["text"].toString();

    QString html = "";
    for(const auto& v : jsonDoc.array()) {
        QJsonObject obj = v.toObject();
        html += "<div class=\"posterAvatar\"><img src=\"" + obj["user"].toObject()["profile_image_url"].toString()
                + "\"></div>";
        html += "<div class=\"text\">" + obj["text"].toString() + "</div>";
    }

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
