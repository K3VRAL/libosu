#include "object/std/metadata.h"

Metadata oos_metadata_init(void) {
    Metadata metadata = {
        .title = NULL,
        .title_unicode = NULL,
        .artist = NULL,
        .artist_unicode = NULL,
        .creator = NULL,
        .version = NULL,
        .source = NULL,
        .tags = NULL,
        .num_tag = 0,
        .beatmap_id = -1,
        .beatmap_set_id = -1
    };
    return metadata;
}

void oos_metadata_free(Metadata *metadata) {
    if (metadata->title != NULL) {
        free(metadata->title);
        metadata->title = NULL;
    }
    if (metadata->title_unicode != NULL) {
        free(metadata->title_unicode);
        metadata->title_unicode = NULL;
    }
    if (metadata->artist != NULL) {
        free(metadata->artist);
        metadata->artist = NULL;
    }
    if (metadata->artist_unicode != NULL) {
        free(metadata->artist_unicode);
        metadata->artist_unicode = NULL;
    }
    if (metadata->creator != NULL) {
        free(metadata->creator);
        metadata->creator = NULL;
    }
    if (metadata->version != NULL) {
        free(metadata->version);
        metadata->version = NULL;
    }
    if (metadata->source != NULL) {
        free(metadata->source);
        metadata->source = NULL;
    }
    if (metadata->tags != NULL) {
        for (int i = 0; i < metadata->num_tag; i++) {
            free(*(metadata->tags + i));
        }
        free(metadata->tags);
        metadata->tags = NULL;
    }
}