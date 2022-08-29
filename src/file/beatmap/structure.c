#include "file/beatmap.h"

void ofb_structure_setfromstring(Structure *structure, char *string) {
    char *version = strrchr(string, 'v');
    if (version == NULL) {
        return;
    }
    
    memmove(&version[0], &version[1], strlen(version));
    structure->version = (int) strtol(version, NULL, 10);
}

char *ofb_structure_tostring(Structure structure) {
    int size_floor = ou_comparing_size(structure.version);
    int len = strlen("osu file format v") + size_floor + (1 + 1);
    char *output = malloc(len * sizeof(char));
    snprintf(output, len, "osu file format v%d\n", structure.version);
    return output;
}