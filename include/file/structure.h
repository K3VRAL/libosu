#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "general.h"
#include "editor.h"
#include "metadata.h"
#include "difficulty.h"
#include "events.h"
#include "timing_points.h"
#include "colours.h"
#include "hit_objects.h"

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#structure
typedef struct Structure {
    int version;
    General general;
    Editor editor;
    Metadata metadata;
    Difficulty difficulty;
    Events events;
    TimingPoints timing_points;
    Colours colours;
    HitObjects hit_objects;
} Structure;

Structure *osus_init(char *);
void osus_free(Structure *);

#endif