#ifndef OFB_METADATA_H
#define OFB_METADATA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "object/std/metadata.h"
#include "unrelated/comparing.h"

/*
    Converts from a string to a Metadata object

    argsreturn
        Metadata *
    args
        char *
*/
void ofb_metadata_setfromstring(Metadata *, char *);

/*
    Converts from a Metadata object to a string

    argsreturn
        char *
    args
        Metadata
*/
void ofb_metadata_tostring(char *, Metadata);

#endif