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

/*
    Converts from a string to a HitObject object

    argsreturn
        HitObject *
    args
        char *
*/
void ofb_hitobject_addfromstring(HitObject *, char *);

/*
    Converts from a HitObject object to a string

    argsreturn
        char *
    args
        HitObject
*/
void ofb_hitobject_tostring(char *, HitObject);

#endif