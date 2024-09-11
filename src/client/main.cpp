#include "client.hpp"
#include <spdlog/spdlog.h>
#include <string>

int main(int argc, char** argv) {
    if (argc != 3) {
        spdlog::error("Usage: ./client <name> <port>");
        return 1;
    }

    try {
        int port = std::stoi(argv[2]);
        if (port <= 0 || port > 65535) {
            throw std::invalid_argument("Invalid port number");
        }

        core::client cli(argv[1], port);
    } catch (const std::exception& e) {
        spdlog::error("unhandled exception: {}", e.what());
        return EXIT_FAILURE;
    } catch (...) {
        spdlog::error("unhandled exception");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}