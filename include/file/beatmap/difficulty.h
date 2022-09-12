#ifndef OFB_DIFFICULTY_H
#define OFB_DIFFICULTY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "object/std/difficulty.h"
#include "unrelated/comparing.h"

/*
    Converts from a string to a Difficulty object

    argsreturn
        Difficulty *
    args
        char *
*/
void ofb_difficulty_setfromstring(Difficulty *, char *);

/*
    Converts from a Difficulty object to a string

    argsreturn
        char *
    args
        Difficulty
*/
void ofb_difficulty_tostring(char *, Difficulty);

#endif