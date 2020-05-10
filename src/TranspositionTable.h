#ifndef CHESSENGINE_TRANSPOSITIONTABLE_H
#define CHESSENGINE_TRANSPOSITIONTABLE_H

#include "Transposition.h"

namespace Engine {
    class TranspositionTable {

    private:
        Transposition *nodes;
        int table_size;
        int getIndex(uint64_t zobrist_hash);//hash function
    public:
        TranspositionTable(int entries);
        void addEntry(const Transposition&transposition);
        Transposition getTransposition(uint64_t zobrist_hash);
        virtual ~TranspositionTable();
    };
}

#endif
