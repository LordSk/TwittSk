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
    QTextStream strim(&html, QIODevice::WriteOnly);

    strim << "<body>";

    for(const auto& v : jsonDoc.array()) {
        QJsonObject obj = v.toObject();
        QJsonObject user = obj["user"].toObject();

        strim
        << "<div class=\"tweetBody\">"

        << "<div class=\"leftPanel\">"
        << "<div class=\"avatar\"><img src=\"" << user["profile_image_url"].toString() << "\"></div>"
        << "</div>"

        << "<div class=\"rightPanel\">"
        << "<div class=\"posterName\">" << user["name"].toString() << "</div>"
        << "<div class=\"text\">" << obj["text"].toString() << "</div>"
        << "</div>"

        << "</div>";
    }

    strim << "</body>";


    _ui->webView->setHtml(html);
    _ui->webView->settings()->setUserStyleSheetUrl(QUrl::fromLocalFile(QDir::currentPath() + "/twittsk.css"));
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
