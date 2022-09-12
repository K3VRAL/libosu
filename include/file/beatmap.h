#ifndef OF_BEATMAP_H
#define OF_BEATMAP_H

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "beatmap/structure.h"
#include "beatmap/general.h"
#include "beatmap/editor.h"
#include "beatmap/metadata.h"
#include "beatmap/difficulty.h"
#include "beatmap/event.h"
#include "beatmap/timing_point.h"
#include "beatmap/colour.h"
#include "beatmap/hit_object.h"

#include "unrelated/reading_line.h"

typedef struct Beatmap {
    Structure structure;
    General general;
    Editor editor;
    Metadata metadata;
    Difficulty difficulty;
    
    Event *events;
    unsigned int num_event;
    
    TimingPoint *timing_points;
    unsigned int num_tp;
    
    Colour *colours;
    unsigned int num_colour;
    
    HitObject *hit_objects;
    unsigned int num_ho;
} Beatmap;

/*
    Allows for the Beatmap struct to be easily initialised

    argsreturn
        Beatmap *
*/
void of_beatmap_init(Beatmap *);

/*
    Allows for the Beatmap struct to easily free all the data it houses

    args
        Beatmap
*/
void of_beatmap_free(Beatmap);

/*
    Converts from a file path (usually an `.osu` file) to a Beatmap object

    argsreturn
        Beatmap *
    args
        char *
*/
void of_beatmap_set(Beatmap *, char *);

/*
    Converts from a Beatmap object to a file output

    argsreturn
        FILE *
    args
        Beatmap
*/
void of_beatmap_tofile(FILE *, Beatmap);

#endif