#include "client.hpp"
#include <objects/message.hpp>
#include <objects/msg_objects.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

namespace core
{
    client::client(std::string cli_name, int con_port) : _ioc(THREADS), _socket(_ioc), _id(0), _port(con_port), _cli_name(cli_name) 
        { start(); }

    client::~client() {}

    std::string client::read_response()
    {
        std::size_t msg_size;
        boost::asio::read(_socket, boost::asio::buffer(&msg_size, sizeof(size_t)));
        
        if(_recv_msg.size() != msg_size)
            _recv_msg.resize(msg_size);

        boost::asio::read(_socket, boost::asio::buffer(_recv_msg.data(), msg_size));
        return std::string(_recv_msg.begin(), _recv_msg.end());
    }

    void client::write(std::string& data)
    {
        core::message msg;

        if (data.empty())
            throw std::runtime_error("data for write is empty");
        else
            msg.data = data;

        _write_buff = core::serialize_message(msg);
        boost::asio::write(_socket, boost::asio::buffer(_write_buff.data(), _write_buff.size()));
    }

    void client::handler_result(const core::msg& rpl)
    {
        if(rpl.message == "auth_message")
        {
            spdlog::info("Get auth message, send name {}", _cli_name);
            std::string r = "auth_message:" + _cli_name;
            write(r);
            return;
        }
        
        spdlog::info("Response: {}", rpl.message);
    }

    void client::executing()
    {
        read(); // Read auth

        core::message msg;
        core::command comm;
        spdlog::info("Enter command");
        while (true)
        {
            std::getline(std::cin, msg.data);
            if (msg.data.empty())
                continue;

            comm.set_command(msg.data);
            comm.params.push_back(std::to_string(_id));
            switch (comm.instruction)
            {
                case command::type::end:
                {
                    std::string tmp("end");
                    write(tmp);
                    stop();
                    return;
                }
                case command::type::ping:
                {
                    std::string tmp("ping");
                    write(tmp);
                    break;
                }
                default:
                {
                    write(msg.data);
                    break;
                }
            }
            if(!read()) break;
            comm.params.clear();
        }
    }

    void client::connect()
    {
        auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), _port);
        _socket.connect(endpoint);
    }

    bool client::read()
    {
        core::msg rpl;
        try
        {
            rpl.set_message(read_response());
        }
        catch (const std::exception& e)
        {
            spdlog::error("Error: {}", e.what());
            stop();
            return false;
        }

        handler_result(rpl);
        return true;
    }

    void client::start()
    {
        create_thread_pool();
        connect();
        (void)std::async(std::launch::async, &client::executing, this);
    }

    void client::stop()
    {
        _ioc.stop();

        for (auto& thread : _threads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }

        _socket.close();

        spdlog::info("Shutdown client");
    }

    void client::create_thread_pool()
    {
        for(auto i = 4 - 1; i > 0; --i)
        {
            _threads.emplace_back([this]{ _ioc.run(); });
        }
        _ioc.run();
    }
}
