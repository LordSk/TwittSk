#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private:
    void closeEvent(QCloseEvent* ce);

    std::unique_ptr<Ui::MainWindow> _ui;
    std::unique_ptr<QNetworkAccessManager> _netMngr;

private slots:
    void replyFinished(QNetworkReply* reply);
};

#endif // MAINWINDOW_H
