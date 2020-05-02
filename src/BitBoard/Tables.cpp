#include "Tables.h"

namespace BitEngine {

    namespace Tables {
        uint64_t AttackTables[64][8];

        void initializeAttackTables() {
            //NORTH
            for (int i = 55; i >= 0; i--) {
                AttackTables[i][North] = (1LL << (i + 8)) | AttackTables[i + 8][North];
            }
            // EAST
            for (int i = 0; i < 64; i++) {
                if (i % 8)
                    AttackTables[i][East] = (1LL << (i - 1)) | AttackTables[i - 1][East];
            }
            //SOUTH
            for (int i = 8; i < 64; i++) {
                AttackTables[i][South] = (1LL << (i - 8)) | AttackTables[i - 8][South];
            }
            //West
            for (int i = 63; i >= 0; i--) {
                if ((i + 1) % 8) {
                    AttackTables[i][West] =(1LL << (i + 1)) | AttackTables[i + 1][West];
                }

            }
        }
    }

}
