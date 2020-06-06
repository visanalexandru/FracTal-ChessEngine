#include "Protocol.h"


Protocol::Protocol(Engine::Board &internal_board) : isRunning(false),
                                                    board(internal_board),eval(internal_board) {


}

void Protocol::send(const std::string &to_send) {

    Engine::Logger::getInstance().log("SEND: " + to_send);
    std::cout << to_send << '\n';
}


void Protocol::init() {
    send("id name StockFischer");
    send("id author Visan");
    send("uciok");
}

void Protocol::setUpPosition() {
    std::string option;
    stream >> option;//the first string is a fen string or "startpos"
    if (option == "startpos") {
        board.loadFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    } else {//it is a fen string
        std::string fen;
        std::string remaining;
        //the fen string is made out of 6 chunks
        for(int i=0;i<6;i++){
            stream>>remaining;
            fen+=remaining+" ";
        }
        board.loadFen(fen);
    }
    if (!stream.eof()) {//has not reached end
        std::string move;
        stream >> move;
        if(move=="moves"){//we need to parse moves
            while (stream>>move) {//parse moves
                Engine::Move server_move = Engine::AnParser::getMove(move, board);
                board.makeMove(server_move);
            }
        }

    }
}

void Protocol::searchMove() {
    std::string aux;
    int time = 10000000, increment = 0;
    int fixed_time = 0;
    Engine::Logger::getInstance().log("Begin search");

    while (stream >> aux) {//we read search options
        if ((aux == "wtime" && board.getTurn() == Engine::White) ||
            (aux == "btime" && board.getTurn() == Engine::Black))
            stream >> time;

        if ((aux == "winc" && board.getTurn() == Engine::White) ||
            (aux == "binc" && board.getTurn() == Engine::Black))
            stream >> increment;
        if (aux == "movetime")
            stream >> fixed_time;
    }

    Engine::Logger::getInstance().log("--remaining time " + std::to_string(time));

    float alloted;
    if (fixed_time)
        alloted = (float) (fixed_time) / 1000.f;
    else alloted = Engine::TimeManager::getTimePerMove(time, increment);

    Engine::Logger::getInstance().log("--alloted " + std::to_string(alloted) + " seconds ");

    float a = clock();
    Engine::Move bestmove = eval.getBestMove(alloted);
    a = (clock() - a) / CLOCKS_PER_SEC;
    Engine::Logger::getInstance().log("--found in " + std::to_string(a) + ": "+bestmove.toString());
    send("bestmove " + bestmove.toString());
}

void Protocol::sendOk() {
    send("readyok");
}

void Protocol::handleRequest(const std::string &req) {

    Engine::Logger::getInstance().log("RECEIVE: " + req);
    stream.clear();
    stream.str(req);

    std::string cmmd;
    stream >> cmmd;

    if (cmmd == "uci") {
        init();
    } else if (cmmd == "isready") {
        sendOk();
    }
    else if (cmmd == "position") {
        setUpPosition();
        Engine::Logger::getInstance().log(board.toString());

    }
    else if (cmmd == "go") {
        searchMove();
    }
    else if (cmmd == "quit") {
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


