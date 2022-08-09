#ifndef OFB_METADATA_H
#define OFB_METADATA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#metadata
typedef struct Metadata {
    char *title;
    char *title_unicode;
    char *artist;
    char *artist_unicode;
    char *creator;
    char *version;
    char *source;
    char **tags;
    unsigned int num_tag;
    int beatmap_id;
    int beatmap_set_id;
} Metadata;

Metadata ofb_metadata_init();
void ofb_metadata_set(Metadata *, char *);
void ofb_metadata_free(Metadata *);
void ofb_metadata_tofile(Metadata, FILE *);

#endif