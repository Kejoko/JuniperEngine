#include "spdlog/spdlog.h"
#include "spdlog/async.h"

#include <memory>

namespace jun {
    class Logger;
}

// A singleton asynch logger using spdlog
class jun::Logger {
    public:
        static Logger& getInstance();

        void logTrace(std::string message);
        void logDebug(std::string message);
        void logInfo(std::string message);
        void logWarn(std::string message);
        void logError(std::string message);
        void logCritical(std::string message);

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
    private:
        Logger();

        std::shared_ptr<spdlog::async_logger> logger;
};