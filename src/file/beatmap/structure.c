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

void ofb_structure_tofile(Structure structure, FILE *fp) {
    char *output = malloc((strlen("osu file format v") + (structure.version == 0 ? 1 :(floor(log10((structure.version)) + 1 + (structure.version < 0 ? 1 : 0))) + 2)) * sizeof(char));
    sprintf(output, "osu file format v%d\n", structure.version);
    fputs(output, fp);
    free(output);
    fputs("\n", fp);
}