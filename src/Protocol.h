#ifndef CHESSENGINE_PROTOCOL_H
#define CHESSENGINE_PROTOCOL_H


#include<iostream>
#include<fstream>
#include<sstream>
#include "AnParser.h"
#include "Eval.h"
#include "TimeManager.h"

class Protocol {
private:
    Engine::Board &board;
    Engine::Eval eval;
    bool isRunning;

    void handleRequest(const std::string &req);

    void send(const std::string &to_send);

    std::stringstream stream;

    void setUpPosition();

    void init();

    void sendOk();

    void searchMove();

public:
    Protocol(Engine::Board &internal_board);

    void start();
};

#endif
