#ifndef OFB_STRUCTURE_H
#define OFB_STRUCTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "object/std/structure.h"
#include "unrelated/comparing.h"

void ofb_structure_setfromstring(Structure **, char *);
void ofb_structure_tostring(char *, Structure *);

#endif