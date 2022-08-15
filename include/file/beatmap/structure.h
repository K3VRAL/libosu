#ifndef OFB_STRUCTURE_H
#define OFB_STRUCTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "unrelated/comparing.h"

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#structure
typedef struct Structure {
    int version;
} Structure;

Structure ofb_structure_init();
void ofb_structure_set(Structure *, char *);
void ofb_structure_free(Structure *);
void ofb_structure_tofile(Structure *, FILE *);

#endif