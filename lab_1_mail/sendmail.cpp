#include "sendmail.h"
#include "ui_sendmail.h"
#include <QDebug>
#include <QMessageBox>
SendMail::SendMail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendMail)
{
    ui->setupUi(this);
}

SendMail::~SendMail()
{
    delete ui;
}

void SendMail::on_buttonBox_accepted()
{
    QString name_user = this->ui->lineEdit->text();
    QString title = this->ui->lineEdit_2->text();
    QString text = this->ui->plainTextEdit->toPlainText();
    if (!name_user.isEmpty() && !title.isEmpty() && !text.isEmpty()) {
        this->text = text;
        send_name = name_user;
        this->title = title;
        this->accept();
    } else {
        QMessageBox::critical(this, QObject::tr("Ошибка"), QObject::tr("Не все поля заполнены!!!"));
    }
}
