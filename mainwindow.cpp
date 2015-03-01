#include <QUrl>
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

    connect(&_updateTimer, SIGNAL(timeout()), this, SLOT(updateTimelines()));
    _updateTimer.start(60*1000);

    connect(&_homeTimeline, SIGNAL(updateDone(int)), this, SLOT(homeTimelineUpdated(int)));

    // update once
    _homeTimeline.update();
}

MainWindow::~MainWindow()
{

}

void MainWindow::showUnreadIcon(int amount)
{
    if(amount == 0) {
        setWindowIcon(_baseIcon);
        return;
    }

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

void MainWindow::showEvent(QShowEvent *event)
{
    event->accept();
    _homeTimeline.setRead();
    showUnreadIcon(0);
}

/*void MainWindow::closeEvent(QCloseEvent *ce)
{
    ce->ignore();
    showMinimized();
}*/

void MainWindow::homeTimelineUpdated(int newTweetsCount)
{
    qDebug() << "Home timeline updated";
    showUnreadIcon(newTweetsCount);

    if(newTweetsCount == 0) // nothing new, don't update the view
        return;

    QString html = "";
    QTextStream out(&html, QIODevice::WriteOnly);

    out
    << "<!DOCTYPE html>"
    << "<html>"
    << "<head>"
    << "<link rel=\"stylesheet\" type=\"text/css\" href=\"" << "file:///" + QDir::currentPath() + "/twittsk.css" << "\">"
    << "</head>"
    << "<body>"

    << _homeTimeline.getHTML();

    out << "</body></html>";


    _ui->webView->setHtml(html);
}

void MainWindow::updateTimelines()
{
    _homeTimeline.update();
}
