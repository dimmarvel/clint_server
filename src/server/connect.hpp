#pragma once
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <boost/bind/bind.hpp>
#include <objects/commands.hpp>
#include <server/connect.hpp>

namespace net
{
    class con_handler : public std::enable_shared_from_this<con_handler>
    {
    public:
        typedef std::shared_ptr<con_handler> ptr;
        explicit con_handler(boost::asio::io_service& io_service);
        ~con_handler() {}
        
        boost::signals2::signal<void(ptr conn, std::string)> on_msg;

    private:
        void handle_write(const boost::system::error_code& err, size_t byte_transferred);
        void accept_message();
        void read_message();
        void on_msg_ready();
        void say_hello();

    public:
        void start();
        static ptr create(boost::asio::io_service& io_service);
        void send(const std::string& message);
        void drop();

        boost::asio::ip::tcp::socket& get_socket() { return _sock; }
        std::string get_adress() { return _addr;}
        std::string get_name() { return _name; }
        void set_name(const std::string& name) { _name = name; }

    private:
        std::string _name;
        std::string _addr;
        boost::asio::ip::tcp::socket _sock;
        boost::asio::streambuf _buf;
        std::vector<uint8_t> _read_buff;
        std::vector<uint8_t> _write_buff;
        std::size_t _msg_size;
    };
}
