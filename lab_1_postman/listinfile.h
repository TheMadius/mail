#ifndef LISTINFILE_H
#define LISTINFILE_H

#include <QFile>
#include <QList>
#include <QDataStream>
#include <iostream>
#include <string>

class ListInFile
{
public:
    ListInFile(std::string);
    ~ListInFile();

    void addMsg(QString msg);
    int getCount();

    QString getMsg();

private:
    QByteArray getDataInFile(QString file_name);
    void createFile(std::string name);
    QString file_name;
    QFile* file;
};

#endif // LISTINFILE_H
