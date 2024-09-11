#include "hub.hpp"
#include "app.hpp"
#include "logger.hpp"
#include "server.hpp"
#include <objects/commands.hpp>
#include <objects/msg_objects.hpp>
#include <spdlog/spdlog.h>
#include <boost/bind.hpp>

namespace core
{
    hub::hub(app &application) : _application(application) {}

    void hub::start()
    {
        spdlog::info("Start hub");
        _log = logger_wrap::get();
        _server = std::make_shared<core::server>(_application.get_service(), _application.get_port());
        _server->start();
        subscribe_on_server();
    }

    void hub::subscribe_on_server()
    {
        _server->on_accept_connection.connect(boost::bind(&hub::on_new_connection, this, _1));
    }

    void hub::on_new_connection(net::con_handler::ptr conn)
    {
        subscribe_on_connection(conn);
    }

    void hub::subscribe_on_connection(net::con_handler::ptr conn)
    {
        conn->on_msg.connect(boost::bind(&hub::on_new_msg, this, _1, _2));
    }

    void hub::on_new_msg(net::con_handler::ptr conn, std::string msg_data)
    {
        msg rpl;
        _log->info("{} {}", conn->get_name(), msg_data);
        command::type t = to_command(msg_data);
        switch (t)
        {
            case command::ping:
            {
                rpl.message ="pong";
                spdlog::info("Server sending pong");
                break;
            }
            case command::end:
            {
                spdlog::info("Client {} disconnect", conn->get_adress());
                conn->drop();
                return;
            }
            case command::auth:
            {
                spdlog::info("Get auth message");
                conn->set_name(parse_auth(msg_data));
                return;
            }
            default:
            {
                rpl.message = msg_data;
                break;
            }
        }
        conn->send(rpl.message);
    }

    std::string parse_auth(const std::string& message) {
        const std::string prefix = "auth_message:";
        const size_t prefix_length = prefix.length();
        
        if (message.substr(0, prefix_length) == prefix) {
            std::string value = message.substr(prefix_length);

            if (value.length() > 30) {
                spdlog::error("Error: Value exceeds 30 characters.");
                return ""; 
            }

            return value;
        }
        
        spdlog::error("Message does not start with 'auth_message:'.");
        return "";
    }

}