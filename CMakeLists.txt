cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)


set(sources src/Protocol.cpp src/AnParser.cpp src/Board.cpp src/Board.h src/Tables.h src/Types.cpp src/Types.h src/MoveGen.cpp src/MoveGen.h src/Move.cpp src/Move.h src/GameState.cpp src/Utils.cpp src/Utils.h src/Tables.cpp)
# set the project name
project(ChessEngine)
# add the executable
add_executable(ChessEngine src/main.cpp  ${sources})
target_compile_options(ChessEngine PRIVATE -g)

add_executable(Tests tests/main.cpp ${sources})
target_compile_options(Tests PRIVATE)
