#include "dialogauthorization.h"
#include "ui_dialogauthorization.h"
#include "hv/requests.h"
#include <QDialog>
#include <QMessageBox>

DialogAuthorization::DialogAuthorization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAuthorization)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
}

DialogAuthorization::~DialogAuthorization()
{
    delete ui;
}

void DialogAuthorization::on_pushButton_clicked()
{
    try {
        QString ip = this->ui->lineEdit->text();
        QString login = this->ui->lineEdit_2->text();

        if (ip.isEmpty() || login.isEmpty()) {
            QMessageBox::critical(this, QObject::tr("Ошибка"), QObject::tr("Не все поля введены!!!"));
        } else {
            auto resp = requests::get(("http://" + ip + "/ping").toStdString().c_str());
            if(resp != NULL) {
                this->host = ip;
                json j_obj;
                j_obj["login"] = login.toStdString();
                auto resp_user = requests::post(("http://" + ip + "/auth").toStdString().c_str(), j_obj.dump());
                if (resp_user != NULL) {
                    if(resp_user->status_code == 200) {
                        json j_resp = json::parse(resp_user->Body());
                        std::string uuid = j_resp["uuid"];
                        this->uuid = uuid.c_str();
                        this->accept();
                    } else {
                        QMessageBox::critical(this, QObject::tr("Ошибка"), QObject::tr("Ошибка авторизации!!!"));
                    }
                } else {
                    QMessageBox::critical(this, QObject::tr("Ошибка"), QObject::tr("Ошибка авторизации!!!"));
                }
            } else {
                QMessageBox::critical(this, QObject::tr("Ошибка"), QObject::tr("Cервер недоступен!!!"));
            }
        }

    }  catch (...) {
        QMessageBox::critical(this, QObject::tr("Ошибка"), QObject::tr("Ошибка авторизации!!!"));
    }

}


void DialogAuthorization::on_pushButton_2_clicked()
{
    this->close();
}

