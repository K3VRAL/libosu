#ifndef OOS_COLOUR_H
#define OOS_COLOUR_H

#include <stdlib.h>

typedef struct Colour {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} Colour;

void oos_colour_free(Colour *colour);

#endif