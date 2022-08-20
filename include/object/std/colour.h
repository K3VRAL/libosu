#ifndef OOS_COLOUR_H
#define OOS_COLOUR_H

#include <stdlib.h>

typedef struct Colour {
    char red;
    char green;
    char blue;
} Colour;

void oos_colour_free(Colour *);

#endif