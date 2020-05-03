cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)


set(sources src/Protocol.cpp src/Board.cpp src/Move.cpp src/Position.cpp src/AnParser.cpp src/GameState.cpp src/MoveGen.cpp src/MoveGen.h src/Types.cpp src/BitBoard/Board.cpp src/BitBoard/Board.h src/BitBoard/Tables.h src/BitBoard/Types.cpp src/BitBoard/Types.h src/BitBoard/MoveGen.cpp src/BitBoard/MoveGen.h src/BitBoard/Move.cpp src/BitBoard/Move.h src/BitBoard/GameState.cpp src/BitBoard/Utils.cpp src/BitBoard/Utils.h src/BitBoard/Tables.cpp)
# set the project name
project(ChessEngine)
# add the executable
add_executable(ChessEngine src/main.cpp  ${sources})
target_compile_options(ChessEngine PRIVATE -g)

add_executable(Tests tests/main.cpp ${sources})
target_compile_options(Tests PRIVATE -O3  -fno-rtti -ansi  -m64  -msse -flto )
