cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)


set(sources src/Protocol.cpp src/AnParser.cpp src/Board.cpp src/Board.h src/Tables.h src/Types.cpp src/Types.h src/MoveGen.cpp src/MoveGen.h src/Move.cpp src/Move.h src/GameState.cpp src/Utils.cpp src/Utils.h src/Tables.cpp src/Eval.cpp src/Eval.h src/Zobrist.cpp src/Zobrist.h src/TranspositionTable.cpp src/TranspositionTable.h src/Transposition.cpp src/Transposition.h src/TimeManager.cpp src/TimeManager.h src/Logger.cpp src/Logger.h src/BoardEval.cpp src/BoardEval.h)
set(test_sources src/Board.cpp src/Board.h src/Tables.h src/Types.cpp src/Types.h src/MoveGen.cpp src/MoveGen.h src/Move.cpp src/Move.h src/GameState.cpp src/Utils.cpp src/Utils.h src/Tables.cpp src/Zobrist.cpp src/Zobrist.h)

# set the project name
project(ChessEngine)
# add the executable
add_executable(ChessEngine src/main.cpp  ${sources})
target_compile_options(ChessEngine PRIVATE -O3  -fno-rtti -ansi  -m64  -msse -flto )

add_executable(Tests tests/main.cpp ${test_sources})
target_compile_options(Tests PRIVATE -O3  -fno-rtti -ansi  -m64  -msse -flto )
