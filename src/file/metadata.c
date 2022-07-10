#include "file/metadata.h"

Metadata osum_init() {
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

void osum_free(Metadata *metadata) {
    if (metadata->title != NULL) {
        free(metadata->title);
    }
    if (metadata->title_unicode != NULL) {
        free(metadata->title_unicode);
    }
    if (metadata->artist != NULL) {
        free(metadata->artist);
    }
    if (metadata->artist_unicode != NULL) {
        free(metadata->artist_unicode);
    }
    if (metadata->creator != NULL) {
        free(metadata->creator);
    }
    if (metadata->version != NULL) {
        free(metadata->version);
    }
    if (metadata->source != NULL) {
        free(metadata->source);
    }
    if (metadata->tags != NULL) {
        for (int i = 0; i < metadata->num_tag; i++) {
            free(*(metadata->tags + i));
        }
        free(metadata->tags);
    }
}

void osum_set(Metadata *metadata, char *key_value_pair) {
    char *token = strtok(key_value_pair, ":");
    if (token != NULL) {
        char *key = strdup(token);
        char *value = strdup(strtok(NULL, ":"));
        if (*(value + 0) == ' ') {
            value++;
        }
        if (strcmp("Title", key) == 0) {
            metadata->title = strdup(value);
        } else if (strcmp("TitleUnicode", key) == 0) {
            metadata->title_unicode = strdup(value);
        } else if (strcmp("Artist", key) == 0) {
            metadata->artist = strdup(value);
        } else if (strcmp("ArtistUnicode", key) == 0) {
            metadata->artist_unicode = strdup(value);
        } else if (strcmp("Creator", key) == 0) {
            metadata->creator = strdup(value);
        } else if (strcmp("Version", key) == 0) {
            metadata->version = strdup(value);
        } else if (strcmp("Source", key) == 0) {
            metadata->source = strdup(value);
        } else if (strcmp("Tags", key) == 0) {
            metadata->tags = malloc(sizeof(char *));
            token = strtok(value, " ");
            while (token != NULL) {
                metadata->tags = realloc(metadata->tags, (metadata->num_tag + 1) * sizeof(char *));
                *(metadata->tags + metadata->num_tag) = token;
                token = strtok(NULL, " ");
                metadata->num_tag++;
            }
        } else if (strcmp("BeatmapID", key) == 0) {
            metadata->beatmap_id = (int) strtol(value, NULL, 10);
        } else if (strcmp("BeatmapSetID", key) == 0) {
            metadata->beatmap_set_id = (int) strtol(value, NULL, 10);
        }
        free(key);
        free(value);
    }
}