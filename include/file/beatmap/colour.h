#ifndef OFB_COLOUR_H
#define OFB_COLOUR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "object/std/colour.h"
#include "unrelated/comparing.h"

Colour ofb_colour_addfromstring(char *);
char *ofb_colour_tostring(Colour, int);

#endif