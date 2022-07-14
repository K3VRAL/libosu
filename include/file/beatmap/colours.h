#ifndef OFB_COLOURS_H
#define OFB_COLOURS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#colours
typedef struct Colour {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Colour;

void ofb_colours_free(Colour **);
void ofb_colours_add(Colour **, unsigned int *, char *);

#endif