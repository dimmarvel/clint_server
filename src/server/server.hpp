#pragma once

#include <boost/signals2.hpp>
#include "connect.hpp"

namespace core
{
    class server
    {
    public:
        server(boost::asio::io_service& io_service, int port);

        boost::signals2::signal<void(net::con_handler::ptr conn)> on_accept_connection;
        
    private:
        void start_accept();    

    public:
        void start();
        void handle_accept(net::con_handler::ptr connection, const boost::system::error_code& err);

    private:
        boost::asio::ip::tcp::acceptor _acceptor;
        boost::asio::io_service& _io_service;
        int _port;
    };
}
