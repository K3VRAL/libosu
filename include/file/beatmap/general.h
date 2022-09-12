#ifndef OFB_GENERAL_H
#define OFB_GENERAL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "object/std/general.h"
#include "unrelated/comparing.h"

/*
    Converts from a string to a General object

    argsreturn
        General *
    args
        char *
*/
void ofb_general_setfromstring(General *, char *);

/*
    Converts from a General object to a string

    argsreturn
        char *
    args
        General
*/
void ofb_general_tostring(char *, General);

#endif