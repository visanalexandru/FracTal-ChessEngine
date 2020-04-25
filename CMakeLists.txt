cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

# set the project name
project(ChessEngine)

# add the executable
add_executable(ChessEngine  main.cpp Protocol.cpp Board.cpp Move.cpp Position.cpp AnParser.cpp)
target_compile_options(ChessEngine PRIVATE -g)

