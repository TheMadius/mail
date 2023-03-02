#ifndef API_H
#define API_H

#include <QDebug>
#include <QHash>
#include <iostream>
#include <string>
#include <thread>
#include <memory>

#include "hv/HttpServer.h"
#include "hv/HttpService.h"
#include "listinfile.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class api
{
public:
    api(std::string host, int port);
    void start(std::string dir);
private:
    void init_mail(std::string dir);
    QHash<QString ,std::shared_ptr<ListInFile>> list_user;
    QHash<QString , QString> user_uuid;
    QHash<QString , QString> uuid_user;
    std::string ip;
    std::shared_ptr<std::thread> _t;
    int count_thread;
    int port;
};

#endif // API_H
