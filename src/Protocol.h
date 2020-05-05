#ifndef CHESSENGINE_PROTOCOL_H
#define CHESSENGINE_PROTOCOL_H


#include<iostream>
#include<fstream>
#include<sstream>
#include "AnParser.h"

class Protocol {
private:
    Engine::Board &board;
    std::ofstream log;
    bool isRunning;

    void handleRequest(const std::string &req);

    void send(const std::string &to_send);

    void Log(const std::string &to_log);

    static bool compare(Engine::Move a, Engine::Move b);

public:
    Protocol(Engine::Board &internal_board);

    void start();
};

#endif
