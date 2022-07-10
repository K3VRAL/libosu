#include "osuu.h"

Osu osu_init() {
    Osu osu = {
        .structure = NULL
    }
    return osu;
}

void osu_free(Osu *osu) {
    if (osu.structure != NULL) {
        osus_free(osu.structure);
    }
}