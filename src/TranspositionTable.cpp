#include "TranspositionTable.h"
namespace Engine {

    TranspositionTable::TranspositionTable(int entries) : table_size(entries) {
        nodes = new Transposition[entries];
    }

    TranspositionTable::~TranspositionTable() {
        delete[] nodes;
    }

    int TranspositionTable::getIndex(uint64_t zobrist_hash) {
        int result = zobrist_hash % table_size;
        return result;
    }

    void TranspositionTable::addEntry(const Transposition &transposition) {
        int index = getIndex(transposition.getZobristHash());

        Transposition old = nodes[index];
        if (old.getType() == NodeType::Null || old.getDepth() <= transposition.getDepth()) {
            nodes[index] = transposition;
        }
    }

    Transposition TranspositionTable::getTransposition(uint64_t zobrist_hash) {

        int index = getIndex(zobrist_hash);
        Transposition selected = nodes[index];
        if (selected.getType() != NodeType::Null &&selected.getZobristHash()==zobrist_hash) {
            return selected;
        }
        return Transposition();
    }
}