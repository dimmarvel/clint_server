#pragma once

#include <boost/asio.hpp>
#include <memory>

namespace core
{
    class hub;
    const constexpr int THREADS = 4;

    class app : public std::enable_shared_from_this<app>
    {
        public:
            app(int port);
            virtual ~app() = default;

            virtual boost::asio::io_service& get_service()  { return _ioc; }
            virtual int get_port()  { return _port; }
            void create_thread_pool();
            void start();

        private:
            boost::asio::io_service     _ioc;
            std::shared_ptr<hub>        _hub;
            std::vector<std::thread>    _threads;
            int _port;
    };
}
