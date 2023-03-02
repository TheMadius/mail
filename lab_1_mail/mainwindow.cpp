#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogauthorization.h"
#include "hv/requests.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    lable = new QLabel();
    this->ui->statusbar->addWidget(lable);
    lable->setText("У вас 0 новых сообщений");
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, & MainWindow::checkNewMsg);
}

void MainWindow::checkNewMsg()
{
    json j_obj;
    j_obj["uuid"] = user_uuid.toStdString();
    auto resp = requests::request(HTTP_GET,("http://" + host + "/countMessage").toStdString().c_str(), j_obj.dump());
    if (resp != NULL) {
       if(resp->status_code == 200) {
           json j_resp = json::parse(resp->Body());
           int count = j_resp["count"];
           lable->setText("У вас " + QString::number(count) + " новых сообщений");
       }
    }
}

void MainWindow::auth(){
    DialogAuthorization *form = new DialogAuthorization(this);
    form->exec();

    if(form->result() == QDialog::Accepted) {
        this->host = form->host;
        this->user_uuid = form->uuid;
        timer->start(2000);
    } else {
        delete form;
        this->close();
    }
    delete form;
}

MainWindow::~MainWindow()
{
    timer->stop();
    delete timer;
    delete ui;
}

