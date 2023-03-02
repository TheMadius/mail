#include "api.h"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include <QDir>
#include <QFileInfoList>

api::api(std::string host, int port) {
    this->ip = host;
    this->port = port;
}

void api::init_mail(std::string dir) {
    QDir dirr(dir.c_str());
    QFileInfoList dirContent = dirr.entryInfoList(QStringList()
        << "*",
        QDir::Files | QDir::NoDotAndDotDot);
    for(auto file : dirContent) {
        qDebug() << "Найден файл " + file.filePath();
        list_user.insert(file.fileName(), std::make_shared<ListInFile>(file.filePath().toStdString()));
    }
}

void api::start(std::string dir) {
    this->_t = std::make_shared<std::thread>([this, dir](){

        hv::HttpService router;
        init_mail(dir);

        router.POST("/auth",  [&](const HttpContextPtr &ctx)
        {
            qDebug() << "Запрос POST /auth";
            try {
                json j_req = json::parse(ctx->body());
                std::string name = j_req["login"];
                std::string uuid;

                if (!list_user.contains(name.c_str()))
                    list_user.insert(name.c_str(), std::make_shared<ListInFile>(dir + "/" + name));

                if (!user_uuid.contains(name.c_str())) {
                    uuid = boost::uuids::to_string(boost::uuids::random_generator()());
                    user_uuid.insert(name.c_str(), uuid.c_str());
                    uuid_user.insert(uuid.c_str(), name.c_str());
                } else {
                    uuid = user_uuid[name.c_str()].toStdString();
                }
                json resp;
                resp["uuid"] = uuid;
                ctx->send(resp.dump());
                return 200;
            }  catch (...) {
                qDebug() << "Непровельный json";
                return 400;
            }
        });

        router.POST("/sendMessage",  [&](const HttpContextPtr &ctx)
        {
            qDebug() << "Запрос POST /sendMessage";
            try {
                json j_req = json::parse(ctx->body());
                json users = j_req["users"];
                std::string msg = j_req["msg"];

                for(auto user: users) {
                    std::string login = user;
                    if(list_user.contains(login.c_str())) {
                        list_user[login.c_str()]->addMsg(msg.c_str());
                    }
                }
                return 200;
            }  catch (...) {
                qDebug() << "Непровельный json";
                return 400;
            }
        });

        router.GET("/readMessage",  [&](const HttpContextPtr &ctx)
        {
            qDebug() << "Запрос GET /readMessage";
            try {
                json j_req = json::parse(ctx->body());
                std::string uuid = j_req["uuid"];
                std::string msg = j_req["msg"];

                if (uuid_user.contains(uuid.c_str())) {
                    auto login = uuid_user[uuid.c_str()];
                        if(list_user.contains(login)) {
                            auto msg = list_user[login]->getMsg();
                            json resp;
                            resp["msg"] = msg.toStdString();
                            return ctx->send(resp.dump());
                        }
                }
                return 403;
            }  catch (...) {
                qDebug() << "Непровельный json";
                return 400;
            }
        });

        router.GET("/countMessage",  [&](const HttpContextPtr &ctx)
        {
            qDebug() << "Запрос GET /countMessage";
            try {
                json j_req = json::parse(ctx->body());
                std::string uuid = j_req["uuid"];

                if (uuid_user.contains(uuid.c_str())) {
                    auto login = uuid_user[uuid.c_str()];
                        if(list_user.contains(login)) {
                            json resp;
                            resp["count"] = list_user[login]->getCount();
                            return ctx->send(resp.dump());
                        }
                }
                return 403;
            }  catch (...) {
                qDebug() << "Непровельный json";
                return 400;
            }
        });

        router.GET("/ping",  [&](const HttpContextPtr &ctx)
        {
            return 200;
        });

        hv::HttpServer server;
        server.registerHttpService(&router);
        server.setHost(this->ip.c_str());
        server.setPort(this->port);
        server.setThreadNum(4);
        server.run();

    });
}
