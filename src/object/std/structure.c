#include "object/std.h"

void oos_structure_init(Structure **structure) {
    *structure = malloc(sizeof(**structure));
    (*structure)->version = 14;
}

void oos_structure_free(Structure *structure) {
    if (structure != NULL) {
        return;
    }
    free(structure);
}
