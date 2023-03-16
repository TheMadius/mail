#ifndef SENDMAIL_H
#define SENDMAIL_H

#include <QDialog>

namespace Ui {
class SendMail;
}

class SendMail : public QDialog
{
    Q_OBJECT

public:
    explicit SendMail(QWidget *parent = nullptr);
    ~SendMail();
    QString text;
    QString send_name;
    QString title;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SendMail *ui;
};

#endif // SENDMAIL_H
