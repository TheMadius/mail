#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void auth();
    ~MainWindow();

public slots:
    void checkNewMsg();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    QString host;
    QString user_uuid;
    QString login;
    Ui::MainWindow *ui;
    QLabel *lable;
    QTimer *timer;
};
#endif // MAINWINDOW_H
