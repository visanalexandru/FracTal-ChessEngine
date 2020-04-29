cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)


set(sources src/Protocol.cpp src/Board.cpp src/Move.cpp src/Position.cpp src/AnParser.cpp src/GameState.cpp src/MoveGen.cpp src/MoveGen.h src/Types.cpp)
# set the project name
project(ChessEngine)
# add the executable
add_executable(ChessEngine src/main.cpp  ${sources})
target_compile_options(ChessEngine PRIVATE -g)

add_executable(Tests tests/main.cpp ${sources})
target_compile_options(Tests PRIVATE -O2)
