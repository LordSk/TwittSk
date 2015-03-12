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
#include "netrequestfactory.h"
#include "tweet.h"
#include "timelineview.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _baseIcon(QDir::currentPath() + "/icon_64.png"),
    _unreadIconImg(QDir::currentPath() + "/icon_red_64.png")
{
    setMinimumSize({500, 600});
    setMaximumWidth(500);
    setWindowIcon(_baseIcon);

    _webView = std::unique_ptr<TimelineView>(new TimelineView(&_homeTimeline, {500, 600}, this));
    connect(_webView.get(), SIGNAL(newTweets(int)), this, SLOT(showUnreadIcon(int)));

    /*_taskbarBut = std::unique_ptr<QWinTaskbarButton>(new QWinTaskbarButton(this));
    _taskbarBut->setWindow(windowHandle());
    _taskbarBut->setOverlayIcon(winIcon);*/

    connect(&_updateTimer, SIGNAL(timeout()), this, SLOT(updateTimelines()));
    _updateTimer.start(60*1000);

    // fetch once
    _homeTimeline.fetchTop();
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

void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);

    if(event->type() == QEvent::ActivationChange) {
        if(isActiveWindow()) {
            _webView->markAsRead();
            showUnreadIcon(0);
        }
    }
}

/*void MainWindow::closeEvent(QCloseEvent *ce)
{
    ce->ignore();
    showMinimized();
}*/

void MainWindow::updateTimelines()
{
    _homeTimeline.fetchTop();
}
