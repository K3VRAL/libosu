#ifndef OFB_EDITOR_H
#define OFB_EDITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "object/std/editor.h"
#include "unrelated/comparing.h"

void ofb_editor_setfromstring(Editor *, char *);
void ofb_editor_tostring(char *, Editor *);

#endif