#pragma once
#include <boost/signals2.hpp>
#include <server/connect.hpp>
#include <memory>
#include "app.hpp"
#include <spdlog/spdlog.h>

namespace core
{
    class app;
    class server;
    class msg;

    class hub : std::enable_shared_from_this<hub>
    {
    public:
        hub(app& application);
        virtual ~hub() = default;
        
        void start();
        
        //TODO: need unsub
        void subscribe_on_connection(net::con_handler::ptr conn);
        void subscribe_on_server();

        void on_new_msg(net::con_handler::ptr conn, std::string data);
        void on_new_connection(net::con_handler::ptr conn);

    protected:
        app& _application;
        std::shared_ptr<server> _server;
        std::shared_ptr<spdlog::logger> _log;
    };
    
    std::string parse_auth(const std::string& message);
}