// A singleton asynch logger using spdlog

#include <spdlog/spdlog.h>

namespace jun {
    class Logger;
}

class jun::Logger {
    public:
        static Logger& getInstance();

        void log();

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
    private:
        Logger();
        int val;
};