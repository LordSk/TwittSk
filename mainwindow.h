#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIcon>
#include <QNetworkAccessManager>
#include <QTimer>
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

    void showUnreadIcon(int amount);

private:
    //void closeEvent(QCloseEvent* ce);
    void changeEvent(QEvent* event);

    QIcon _baseIcon;
    QImage _unreadIconImg;
    std::unique_ptr<Ui::MainWindow> _ui;
    //std::unique_ptr<class QWinTaskbarButton> _taskbarBut;
    QTimer _updateTimer;
    HomeTimeline _homeTimeline;

private slots:
    void homeTimelineUpdated(int newTweetsCount);
    void updateTimelines();
};

#endif // MAINWINDOW_H
