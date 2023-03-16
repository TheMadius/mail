#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogauthorization.h"
#include "sendmail.h"
#include "hv/requests.h"
#include <QMessageBox>
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
    } else {
        QMessageBox::critical(this, QObject::tr("Ошибка"), QObject::tr("Cервер недоступен!!!"));
        timer->stop();
        this->auth();
    }
}

void MainWindow::auth(){
    DialogAuthorization *form = new DialogAuthorization(this);
    form->exec();

    if(form->result() == QDialog::Accepted) {
        this->host = form->host;
        this->user_uuid = form->uuid;
        this->login = form->name;
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


void MainWindow::on_pushButton_clicked()
{
    SendMail *form = new SendMail(this);
    form->exec();

    if(form->result() == QDialog::Accepted) {
        json msg, data, users;
        auto list_user = form->send_name.split(';');
        for(auto it: list_user) {
            users.push_back(it.toStdString());
        }

        msg["author"] = this->login.toStdString();
        msg["text"] = form->text.toStdString();
        msg["title"] = form->text.toStdString();

        data["users"] = users;
        data["msg"] = msg.dump();

        auto resp = requests::post(("http://" + this->host + "/sendMessage").toStdString().c_str(), data.dump());
        if(resp != NULL) {
            if(resp->status_code == 200) {
                QMessageBox::information(this, "Успех","Сообщение отправлено");
            } else {
                QMessageBox::critical(this, QObject::tr("Ошибка"), "Ошибка cервер: код " + QString::number(resp->status_code));
            }
        } else {
            QMessageBox::critical(this, QObject::tr("Ошибка"), QObject::tr("Cервер недоступен!!!"));
        }
    }
    delete form;
}


void MainWindow::on_pushButton_2_clicked()
{
    auto resp = requests::get(("http://" + this->host + "/readMessage?uuid=" + this->user_uuid).toStdString().c_str());
    if(resp != NULL) {
        if(resp->status_code == 200) {
            json data = json::parse(resp->Body().c_str());
            std::string msg = data["msg"];
            if (msg == "") {
                QMessageBox::information(this, "Сообщения","Сообщений нет");
            } else {
                json j_msg = json::parse(msg);
                std::string author = j_msg["author"];
                std::string text = j_msg["text"];
                std::string title = j_msg["title"];

                QString text_msg = ("От пользователя: " + author + "\nТема: " + title + "\n\n" + text).c_str();
                this->ui->textBrowser->setText(text_msg);
            }
        } else {
            QMessageBox::critical(this, QObject::tr("Ошибка"), "Ошибка cервер: код " + QString::number(resp->status_code));
        }
    } else {
        QMessageBox::critical(this, QObject::tr("Ошибка"), QObject::tr("Cервер недоступен!!!"));
    }
}

