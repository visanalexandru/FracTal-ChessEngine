#ifndef CHESSENGINE_LOGGER_H
#define CHESSENGINE_LOGGER_H

#include <fstream>

namespace Engine {
    class Logger {
    private:
        std::ofstream out;

        Logger();

    public:
        static Logger &getInstance();

        void log(const std::string &to_log);
    };
}


#endif
