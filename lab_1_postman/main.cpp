#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QString>
#include <iostream>
#include <QTextStream>

#include "api.h"
#include "listinfile.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc != 4) {
        qDebug() << "Не указаны параметры запуска";
        exit(1);
    }

    QDir().mkdir(argv[3]);
    qDebug() << "Создана папка " + QString(argv[3]);
    api api_post(argv[1], atoi(argv[2]));
    api_post.start(argv[3]);
    qDebug() << "Запущено API  " + QString(argv[1]) + ":" + QString(argv[2]);

    return a.exec();
}
