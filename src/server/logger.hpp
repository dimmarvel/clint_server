#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>
#include <string>
#include "logger.hpp"

class logger_wrap 
{
public:
    static void init(const std::string& filename, spdlog::level::level_enum level = spdlog::level::info) {
        if (!logger) {
            logger = spdlog::basic_logger_mt(filename, filename);
            logger->set_level(level);
            logger->flush_on(level);
        }
    }

    static std::shared_ptr<spdlog::logger> get() {
        if (!logger) {
            throw std::runtime_error("Logger not initialized. Call CustomLogger::init() first.");
        }
        return logger;
    }

private:
    static std::shared_ptr<spdlog::logger> logger; 
};
