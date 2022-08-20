#ifndef OFB_METADATA_H
#define OFB_METADATA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "object/std/metadata.h"
#include "unrelated/comparing.h"

void ofb_metadata_setfromstring(Metadata *, char *);
char *ofb_metadata_tostring(Metadata);

#endif