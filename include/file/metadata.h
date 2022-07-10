#ifndef METADATA_H
#define METADATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Metadata osum_init();
void osum_free(Metadata *);
void osum_set(Metadata *, char *);

#endif