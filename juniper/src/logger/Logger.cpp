#include "Logger.h"

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

jun::Logger::Logger() {
    spdlog::init_thread_pool(8192, 1);

    spdlog::sink_ptr stdoutSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    stdoutSink->set_level(spdlog::level::trace);

    std::string dateString = "DATE";
    std::string fileName = "junlog." + dateString + ".log";
    spdlog::sink_ptr fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(fileName, true);
    fileSink->set_level(spdlog::level::warn);

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(stdoutSink);
    sinks.push_back(fileSink);

    logger = std::make_shared<spdlog::async_logger>("master logger", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    logger->set_level(spdlog::level::trace);
    spdlog::register_logger(logger);

    spdlog::set_pattern("[%T:%e] [%^%=10l%$] %v");
}

jun::Logger& jun::Logger::getInstance() {
    static Logger instance;
    return instance;
}

void jun::Logger::logTrace(std::string message) {
    logger->trace(message);
}

void jun::Logger::logDebug(std::string message) {
    logger->debug(message);
}

void jun::Logger::logInfo(std::string message) {
    logger->info(message);
}

void jun::Logger::logWarn(std::string message) {
    logger->warn(message);
}

void jun::Logger::logError(std::string message) {
    logger->error(message);
}

void jun::Logger::logCritical(std::string message) {
    logger->critical(message);
}