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

    NetRequestFactory nrf;
    _netMngr->get(nrf.homeTimeline());
}

MainWindow::~MainWindow()
{

}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    qDebug() << "replyFinished()";
    QByteArray all = reply->readAll();
    QString str = QString::fromLatin1(all);
    _ui->textEdit->setText(str);
    reply->disconnect();
    reply->deleteLater();
}
