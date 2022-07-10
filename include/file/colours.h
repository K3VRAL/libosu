#ifndef COLOURS_H
#define COLOURS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#colours
typedef struct Combo {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Combo;

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#structure
typedef struct Colours {
    Combo *combo;
    unsigned int num;
} Colours;

Colours osuc_init();
void osuc_free(Colours *);
void osuc_add_C(Colours *, char *);

#endif