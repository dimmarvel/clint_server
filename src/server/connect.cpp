#include "connect.hpp"
#include <objects/commands.hpp>
#include <objects/message.hpp>
#include <objects/msg_objects.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

namespace net
{
    con_handler::con_handler(boost::asio::io_service& io_service) : 
        _sock(io_service) 
    {}

    void con_handler::on_msg_ready()
    {
        core::message msg = core::deserialize_message(_read_buff, _msg_size);
        
        spdlog::info("Response: {}", msg.data);
        on_msg(shared_from_this(), msg.data);
    }

    void con_handler::say_hello()
    {
        boost::asio::ip::tcp::endpoint remote_endpoint = _sock.remote_endpoint();
        _addr = remote_endpoint.address().to_string() + ":" + std::to_string(remote_endpoint.port());
        spdlog::info("Client {} connected", get_adress());
        send("auth_message");
    }


    void con_handler::read_message()
    {
        if (_read_buff.size() < _msg_size)
            _read_buff.resize(_msg_size);

        auto self = shared_from_this();
        boost::asio::async_read(_sock, boost::asio::buffer(_read_buff.data(), _msg_size),
            [self](const boost::system::error_code& e, std::size_t transferred)
            {
                if (e)
                    spdlog::error("{}", e.message());

                spdlog::warn("{} == {}", transferred, self->_msg_size);
                assert(transferred == self->_msg_size);
                self->on_msg_ready();
            });
    }

    void con_handler::accept_message()
    {
        boost::asio::async_read(_sock,
        boost::asio::buffer(&_msg_size, sizeof(size_t)),
        [self = shared_from_this()](const boost::system::error_code& error, size_t bytes_transferred) {
            if (!error)
            {
                self->read_message();
                (void)std::async(std::launch::async, [self]() {
                    self->accept_message();
                });
            }
            else
            {
                self->_sock.close();
            }
        });
    }

    void con_handler::start()
    {
        say_hello();
        accept_message();
    }

    void con_handler::send(const std::string& data)
    { 
        core::message msg;
        msg.data = data;
        _write_buff = core::serialize_message(msg);

        boost::asio::async_write(_sock,
            boost::asio::buffer(_write_buff.data(), _write_buff.size()),
            boost::bind(&con_handler::handle_write,
            shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }

    void con_handler::handle_write(const boost::system::error_code& err, size_t byte_transferred)
    {
        if (err)
        {
            spdlog::error("Write error: {}", err.message());
            _sock.close();
        }
    }

    net::con_handler::ptr con_handler::create(boost::asio::io_service& io_service) 
    {
        return std::make_shared<con_handler>(io_service);
    }

    void con_handler::drop()
    {
        _sock.close();
    }
}
