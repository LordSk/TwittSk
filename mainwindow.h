#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIcon>
#include <QNetworkAccessManager>
#include <memory>
#include "timeline.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void testHTML();
    void showUnreadIcon(int amount);

private:
    //void closeEvent(QCloseEvent* ce);

    QIcon _baseIcon;
    QImage _unreadIconImg;
    std::unique_ptr<Ui::MainWindow> _ui;
    //std::unique_ptr<class QWinTaskbarButton> _taskbarBut;
    HomeTimeline _homeTl;

private slots:
    void replyFinished(QNetworkReply* reply);
};

#endif // MAINWINDOW_H
