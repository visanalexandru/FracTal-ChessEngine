#include "Logger.h"
#include <iostream>
namespace Engine{
    Logger::Logger():out("chesslog"){
        std::cout<<"new instance"<<std::endl;
    }
    Logger& Logger::getInstance() {
        static Logger instance;
        return instance;
    }
    void Logger::log(const std::string &to_log) {
        out << to_log << std::endl;
    }
}