#include "listinfile.h"
#include <QtDebug>

ListInFile::ListInFile(std::string name)
{
    file_name = name.c_str();
    file = new QFile(name.c_str());
    file->open(QIODevice::ReadOnly);
    if(!file->isOpen()) {
        createFile(name);
    } else {
        file->close();
    }
}

void ListInFile::createFile(std::string name) {
    file->open(QIODevice::WriteOnly);

    QByteArray saveData;
    QDataStream streamVMan(&saveData, QIODevice::WriteOnly);
    QList<QString> emptyList;
    streamVMan << emptyList;
    file->write(saveData);

    file->close();
}

QByteArray ListInFile::getDataInFile(QString file_name)
{
    QByteArray data;
    QFile file(file_name);

    file.open(QIODevice::ReadOnly);
    data = file.readAll();
    file.close();
    return data;
}

QString ListInFile::getMsg() {
    QByteArray data = getDataInFile(file_name);
    QDataStream stream(&data, QIODevice::ReadOnly);

    QList<QString> msgList;
    stream >> msgList;   
    QString msg;

    if (msgList.size() == 0) {
        return "";
    }

    msg = msgList.first();
    msgList.pop_back();

    QByteArray saveData;
    QDataStream streamVMan(&saveData, QIODevice::WriteOnly);
    streamVMan << msgList;

    file->open(QIODevice::WriteOnly);
    file->write(saveData);
    file->close();

    return msg;
}

int ListInFile::getCount() {
    QByteArray data = getDataInFile(file_name);
    QDataStream stream(&data, QIODevice::ReadOnly);

    QList<QString> msgList;
    stream >> msgList;
    QString msg;

    return msgList.size();
}

void ListInFile::addMsg(QString msg) {
    QByteArray data = getDataInFile(file_name);
    QDataStream stream(&data, QIODevice::ReadOnly);
    QList<QString> msgList;
    stream >> msgList;

    msgList.append(msg);

    QByteArray saveData;
    QDataStream streamVMan(&saveData, QIODevice::WriteOnly);
    streamVMan << msgList;

    file->open(QIODevice::WriteOnly);
    file->write(saveData);
    file->close();
}

ListInFile::~ListInFile() {
    delete file;
}
