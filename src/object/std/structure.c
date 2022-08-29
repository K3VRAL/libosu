#include "object/std.h"

Structure oos_structure_init(void) {
    Structure structure = {
        .version = 14
    };
    return structure;
}

void oos_structure_free(Structure structure) {
    // Why not
}
