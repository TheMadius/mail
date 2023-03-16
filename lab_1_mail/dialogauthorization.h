#ifndef DIALOGAUTHORIZATION_H
#define DIALOGAUTHORIZATION_H

#include <QDialog>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace Ui {
class DialogAuthorization;
}

class DialogAuthorization : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAuthorization(QWidget *parent = nullptr);
    ~DialogAuthorization();
    QString uuid;
    QString host;
    QString name;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DialogAuthorization *ui;
};

#endif // DIALOGAUTHORIZATION_H
