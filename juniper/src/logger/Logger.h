#pragma once

#include <memory>

#include "spdlog/async.h"
#include "spdlog/spdlog.h"

namespace jun {
    class Logger;
}

// A singleton asynch logger using spdlog
class jun::Logger {
    public:
        static void init();

        static void trace(std::string message);
        static void debug(std::string message);
        static void info(std::string message);
        static void warn(std::string message);
        static void error(std::string message);
        static void critical(std::string message);

        Logger() = delete;

    private:
        static void assertInitialized();

        static bool initialized;
        static std::shared_ptr<spdlog::async_logger> m_logger;
};