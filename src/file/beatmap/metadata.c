#include "file/beatmap/metadata.h"

Metadata ofb_metadata_init() {
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

void ofb_metadata_free(Metadata *metadata) {
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

void ofb_metadata_set(Metadata *metadata, char *key_value_pair) {
    char *token = strtok(key_value_pair, ":");
    if (token != NULL) {
        char *key = strdup(token);
        char *value = strtok(NULL, ":");
        if (value != NULL) {
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
                metadata->tags = calloc(1, sizeof(char *));
                token = strtok(value, " ");
                while (token != NULL) {
                    metadata->tags = realloc(metadata->tags, (metadata->num_tag + 1) * sizeof(char *));
                    *(metadata->tags + metadata->num_tag) = strdup(token);
                    token = strtok(NULL, " ");
                    metadata->num_tag++;
                }
            } else if (strcmp("BeatmapID", key) == 0) {
                metadata->beatmap_id = (int) strtol(value, NULL, 10);
            } else if (strcmp("BeatmapSetID", key) == 0) {
                metadata->beatmap_set_id = (int) strtol(value, NULL, 10);
            }
        }
        free(key);
    }
}

void ofb_metadata_tofile(Metadata metadata, FILE *fp) {
    fputs("[Metadata]\n", fp);
    struct {
        char *name;
        union {
            char *cp;
            struct {
                char **cpp;
                unsigned int ui;
            } s;
            int n;
        } info;
        enum {
            cp,
            s,
            n
        } type;
    } data[] = {
        {
            .name = "Title",
            .info.cp = metadata.title,
            .type = cp,
        },
        {
            .name = "TitleUnicode",
            .info.cp = metadata.title_unicode,
            .type = cp,
        },
        {
            .name = "Artist",
            .info.cp = metadata.artist,
            .type = cp,
        },
        {
            .name = "ArtistUnicode",
            .info.cp = metadata.artist_unicode,
            .type = cp,
        },
        {
            .name = "Creator",
            .info.cp = metadata.creator,
            .type = cp,
        },
        {
            .name = "Version",
            .info.cp = metadata.version,
            .type = cp,
        },
        {
            .name = "Source",
            .info.cp = metadata.source,
            .type = cp,
        },
        {
            .name = "Tags",
            .info.s.cpp = metadata.tags,
            .info.s.ui = metadata.num_tag,
            .type = s,
        },
        {
            .name = "BeatmapID",
            .info.n = metadata.beatmap_id,
            .type = n,
        },
        {
            .name = "BeatmapSetID",
            .info.n = metadata.beatmap_set_id,
            .type = n,
        },
    };
    for (int i = 0; i < 10; i++) {
        char *output;
        switch ((data + i)->type) {
            case cp: {
                int len = strlen((data + i)->name) + 1 + 1;
                output = malloc((len + 1) * sizeof(char));
                sprintf(output, "%s:", (data + i)->name);
                if ((data + i)->info.cp != NULL) {
                    output = realloc(output, (len + strlen((data + i)->info.cp) + 1) * sizeof(char));
                    strcat(output, (data + i)->info.cp);
                    strcat(output, "\n");
                }
                break;
            }

            case s: {
                int len = strlen((data + i)->name) + 1 + 1;
                output = malloc(len * sizeof(char));
                sprintf(output, "%s:", (data + i)->name);
                if ((data + i)->info.s.cpp != NULL || (data + i)->info.s.ui != 0) {
                    for (int j = 0; j < (data + i)->info.s.ui; j++) {
                        int len_buffer = strlen(*((data + i)->info.s.cpp + j)) + (*((data + i)->info.s.cpp + j + 1) != NULL ? 1 : 0);
                        len += len_buffer;
                        char *buffer = malloc((len_buffer + 1) * sizeof(char));
                        sprintf(buffer, (*((data + i)->info.s.cpp + j + 1) != NULL ? "%s " : "%s"), *((data + i)->info.s.cpp + j));
                        output = realloc(output, len * sizeof(char));
                        strcat(output, buffer);
                        free(buffer);
                    }
                }
                strcat(output, "\n");
                break;
            }

            case n:
                output = malloc((strlen((data + i)->name) + 1 + ((data + i)->info.n == 0 ? 1 : (floor(log10((data + i)->info.n)) + 1 + ((data + i)->info.n < 0 ? 1 : 0)) + 2)) * sizeof(char));
                sprintf(output, "%s:%d\n", (data + i)->name, (data + i)->info.n);
                break;
        }
        fputs(output, fp);
        free(output);
    }
    fputs("\n", fp);
}