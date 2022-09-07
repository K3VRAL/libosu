#ifndef OFB_HITOBJECT_H
#define OFB_HITOBJECT_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "object/std/hit_object.h"
#include "unrelated/comparing.h"

void ofb_hitobject_addfromstring(HitObject **, char *);
void ofb_hitobject_tostring(char *, HitObject *);

#endif