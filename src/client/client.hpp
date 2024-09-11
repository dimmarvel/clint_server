#pragma once
#include <boost/asio.hpp>
#include <objects/commands.hpp>
#include <thread>

namespace core
{
    class msg;
    class command;
    const constexpr int THREADS = 4;

    class client
    {
        public:
            client(std::string cli_name, int con_port);
            ~client();

        public:
            void write(std::string& msg);
            bool read();
            void connect();
            void handler_result(const core::msg& rpl);
            std::string read_response();
            void stop();
            void start();
            void executing();
            void create_thread_pool();

        private:
            boost::asio::io_service _ioc;
            boost::asio::ip::tcp::socket _socket;
            boost::asio::streambuf _buf;
            std::vector<uint8_t> _recv_msg;
            std::vector<uint8_t> _write_buff;
            std::vector<std::thread> _threads;
            uint64_t _id;
            int _port{8080};
            std::string _cli_name{"Unnamed"};
    };
}
