#ifndef PROTOCOL_H
#define PROTOCOL_H


#include<iostream>
#include<fstream>
#include<sstream>
#include "AnParser.h"

class Protocol {
private:
    BitEngine::Board &board;
    std::ofstream log;
    bool isRunning;

    void handleRequest(const std::string &req);

    void send(const std::string &to_send);

    void Log(const std::string &to_log);

    static bool compare(BitEngine::Move a,BitEngine::Move b);

public:
    Protocol(BitEngine::Board &internal_board);

    void start();
};

#endif
