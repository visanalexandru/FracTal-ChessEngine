#include "Protocol.h"


Protocol::Protocol(Engine::Board &internal_board) : isRunning(false),
                                                    log("chesslog"),
                                                    board(internal_board) {


}

void Protocol::send(const std::string &to_send) {
    Log("sent " + to_send);
    std::cout << to_send << '\n';
}

void Protocol::Log(const std::string &to_log) {
    log << to_log << std::endl;
}

bool Protocol::compare(Engine::Move a, Engine::Move b) {
    return a.getType() < b.getType();
}

void Protocol::handleRequest(const std::string &req) {

    log << "received " << req << std::endl;

    std::stringstream sstream;
    sstream << req;

    std::string cmmd;
    sstream >> cmmd;

    if (cmmd == "uci") {
        send("id name StockFischer");
        send("id author Visan");
        send("uciok");
    } else if (cmmd == "isready") {
        send("readyok");
    } else if (cmmd == "position") {
        std::string aux;
        sstream >> aux;//the first string is a fen string or "startpos"
        if(aux=="startpos"){
            board.loadFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        }
        else{
            board.loadFen(aux);
        }
        if(!sstream.eof()){
            sstream>>aux;//moves
            while (!sstream.eof()){
                sstream>>aux;
                Engine::Move server_move = Engine::AnParser::getMove(aux, board);
                board.makeMove(server_move);
            }
        }

    } else if (cmmd == "go") {
        Engine::Eval eval(board);

        float a =clock();
        Engine::Move bestmove =eval.getBestMove(10);
        a=(clock()-a)/CLOCKS_PER_SEC;
        Log("found in "+std::to_string(a));
        send("bestmove " + bestmove.toString());
        board.makeMove(bestmove);
        Log(board.toString());
    } else if (cmmd == "quit") {
        isRunning = false;
    }
}

void Protocol::start() {
    std::string input;
    isRunning = true;
    while (isRunning) {
        std::getline(std::cin, input);
        handleRequest(input);
    }
}


