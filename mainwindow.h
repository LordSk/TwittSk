#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIcon>
#include <QNetworkAccessManager>
#include <memory>

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
    std::unique_ptr<QNetworkAccessManager> _netMngr;
    //std::unique_ptr<class QWinTaskbarButton> _taskbarBut;

private slots:
    void replyFinished(QNetworkReply* reply);
};

#endif // MAINWINDOW_H
