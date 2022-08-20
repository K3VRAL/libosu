#ifndef OFB_DIFFICULTY_H
#define OFB_DIFFICULTY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "object/std/difficulty.h"
#include "unrelated/comparing.h"

void ofb_difficulty_setfromstring(Difficulty *, char *);
char *ofb_difficulty_tostring(Difficulty);

#endif