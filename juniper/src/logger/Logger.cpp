#include "Logger.h"

#include "Core.h"

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

bool jun::Logger::initialized = false;
std::shared_ptr<spdlog::async_logger> jun::Logger::logger = nullptr;

void jun::Logger::init() {
    if (initialized) return;
    
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
    #if defined(BUILD_DEBUG)
    logger->set_level(spdlog::level::trace);
    #elif defined(BUILD_TEST)
    logger->set_level(spdlog::level::info);
    #elif defined(BUILD_RELEASE)
    logger->set_level(spdlog::level::err);
    #endif

    spdlog::register_logger(logger);

    spdlog::set_pattern("[%T:%e] [%^%=10l%$] %v");

    initialized = true;
    logger->info("Logger initialized");
}

void jun::Logger::assertInitialized() {
    if (!initialized) {
        #if defined(BUILD_DEBUG) || defined(BUILD_TEST)
        std::cerr << "jun::Logger not initialized\n";
        #endif
        throw std::runtime_error("jun::Logger not initialized");
    }
}

void jun::Logger::trace(std::string message) {
    assertInitialized();
    logger->trace(message);
}

void jun::Logger::debug(std::string message) {
    assertInitialized();
    logger->debug(message);
}

void jun::Logger::info(std::string message) {
    assertInitialized();
    logger->info(message);
}

void jun::Logger::warn(std::string message) {
    assertInitialized();
    logger->warn(message);
}

void jun::Logger::error(std::string message) {
    assertInitialized();
    logger->error(message);
}

void jun::Logger::critical(std::string message) {
    assertInitialized();
    logger->critical(message);
}