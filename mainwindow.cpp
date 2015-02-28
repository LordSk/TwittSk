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
#include <QWinTaskbarButton>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "netrequestfactory.h"
#include "tweet.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _baseIcon(QDir::currentPath() + "/icon_64.png"),
    _unreadIconImg(QDir::currentPath() + "/icon_red_64.png"),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    setMinimumSize(size());
    setWindowIcon(_baseIcon);

    /*_taskbarBut = std::unique_ptr<QWinTaskbarButton>(new QWinTaskbarButton(this));
    _taskbarBut->setWindow(windowHandle());
    _taskbarBut->setOverlayIcon(winIcon);*/

    testHTML();
    showUnreadIcon(6);
    _homeTl.update();
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

void MainWindow::showUnreadIcon(int amount)
{
    QImage newIcon(_unreadIconImg);
    QPainter painter;

    painter.begin(&newIcon);

    QFont font;
    font.setPixelSize(19);
    font.setBold(true);

    QStaticText text(QString::number(amount));
    text.prepare();

    painter.setFont(font);
    QPen penHText(QColor("#fff"));
    painter.setPen(penHText);

    int delta = 0;
    if(amount < 10)
        delta = 7;

    painter.drawStaticText(QPointF(36+delta, 37), text);

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
