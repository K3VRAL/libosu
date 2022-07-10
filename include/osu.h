#ifndef OSUU_H
#define OSUU_H

#include <stdio.h>
#include <stdlib.h>

#include "file/structure.h"

typedef struct Osu {
    Structure *structure;
} Osu;

Osu osu_init();
void osu_free(Osu *);

#endif