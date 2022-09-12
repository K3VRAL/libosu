#ifndef OFB_STRUCTURE_H
#define OFB_STRUCTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "object/std/structure.h"
#include "unrelated/comparing.h"

/*
    Converts from a string to a Structure object

    argsreturn
        Structure *
    args
        char *
*/
void ofb_structure_setfromstring(Structure *, char *);

/*
    Converts from a Structure object to a string

    argsreturn
        char *
    args
        Structure
*/
void ofb_structure_tostring(char *, Structure);

#endif