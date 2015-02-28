#include <QUrl>
#include <QFile>
#include <QDir>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>
#include <QIcon>
#include <QCloseEvent>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QStaticText>
#include <QFont>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "netrequestfactory.h"
#include "tweet.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    setMinimumSize(size());
    setWindowIcon(QIcon(QDir::currentPath() + "/icon_64.png"));

    _netMngr = std::unique_ptr<QNetworkAccessManager>(new QNetworkAccessManager());

    connect(_netMngr.get(), SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    /*NetRequestFactory nrf;
    _netMngr->get(nrf.homeTimeline());*/

    testHTML();
    testIcon();
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

void MainWindow::testIcon()
{
    QImage newIcon(QDir::currentPath() + "/icon_red_64.png");
    QPainter painter;

    painter.begin(&newIcon);

    QFont font;
    font.setPixelSize(20);
    font.setBold(true);

    QStaticText text("99");
    text.prepare();

    painter.setFont(font);
    QPen penHText(QColor("#fff"));
    painter.setPen(penHText);

    painter.drawStaticText(QPointF(32, 32), text);

    painter.end();

    setWindowIcon(QIcon(QPixmap::fromImage(newIcon)));
}

/*void MainWindow::closeEvent(QCloseEvent *ce)
{
    ce->ignore();
    showMinimized();
}*/

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
