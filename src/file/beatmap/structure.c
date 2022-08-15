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

void ofb_structure_tofile(Structure *structure, FILE *fp) {
    int size_floor = ou_comparing_size(structure->version);
    int len = strlen("osu file format v") + size_floor + 1;
    char *output = malloc((len + 1) * sizeof(char));
    snprintf(output, len, "osu file format v%d", structure->version);
    strcat(output, "\n");
    fputs(output, fp);
    free(output);
    fputs("\n", fp);
}