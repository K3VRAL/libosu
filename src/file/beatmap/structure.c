#include "file/beatmap/structure.h"

Structure ofb_structure_init() {
    Structure structure = {
        .version = -1
    };
    return structure;
}

void ofb_structure_free(Structure *structure) {
    // Nothing to do
}

void ofb_structure_set(Structure *structure, char *string) {
    char *version = strrchr(string, 'v');
    if (version != NULL) {
        memmove(&version[0], &version[1], strlen(version));
        structure->version = (int) strtol(version, NULL, 10);
    }
}