#ifndef OOS_METADATA_H
#define OOS_METADATA_H

#include <stdlib.h>

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

Metadata oos_metadata_init(void);
void oos_metadata_free(Metadata);

#endif