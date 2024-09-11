#include <objects/msg_objects.hpp>

namespace core
{
    uint64_t msg::id = 0;
    msg::msg() {++id;}
    msg::msg(std::string data) : message(data) {}

    void msg::set_message(const std::string& data)
    {
        message = data;
    }
}