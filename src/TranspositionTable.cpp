#include "TranspositionTable.h"

namespace Engine {
    TranspositionTable &TranspositionTable::getInstance() {
        static TranspositionTable instance;
        return instance;
    }

    TranspositionTable::TranspositionTable() : table_size(0) {

    }

    void TranspositionTable::initSize(int mb) {
        delete[] nodes;
        table_size=(mb*1000000)/ sizeof(Transposition);
        nodes=new Transposition[table_size];
    }

    TranspositionTable::~TranspositionTable() {
        delete[] nodes;
    }

    unsigned TranspositionTable::getIndex(uint64_t zobrist_hash) {
        unsigned result = zobrist_hash % table_size;
        return result;
    }

    void TranspositionTable::addEntry(const Transposition &transposition) {
        unsigned index = getIndex(transposition.getZobristHash());

        Transposition old = nodes[index];
        if (old.getType() == NodeType::Null || old.getDepth() <= transposition.getDepth()) {
            nodes[index] = transposition;
        }
    }

    Transposition TranspositionTable::getTransposition(uint64_t zobrist_hash) {

        unsigned index = getIndex(zobrist_hash);
        Transposition selected = nodes[index];
        if (selected.getType() != NodeType::Null && selected.getZobristHash() == zobrist_hash) {
            return selected;
        }
        return Transposition();
    }
}