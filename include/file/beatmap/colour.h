#ifndef OFB_COLOUR_H
#define OFB_COLOUR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "object/std/colour.h"
#include "unrelated/comparing.h"

/*
    Converts from a string to a Colour object
    
    argsreturn
        Colour *
    args
        char *
*/
void ofb_colour_addfromstring(Colour *, char *);

/*
    Converts from a Colour object to a string

    argsreturn
        char *
    args
        Colour
        int
*/
void ofb_colour_tostring(char *, Colour, int);

#endif