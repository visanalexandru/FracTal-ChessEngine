cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

# set the project name
project(ChessEngine)

# add the executable
add_executable(ChessEngine src/main.cpp src/Protocol.cpp src/Board.cpp src/Move.cpp src/Position.cpp src/AnParser.cpp src/GameState.cpp)
target_compile_options(ChessEngine PRIVATE -g)

