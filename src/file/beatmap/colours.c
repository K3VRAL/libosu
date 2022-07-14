#include "file/beatmap/colours.h"

void ofb_colours_add(Colour **colour, unsigned int *num, char *key_value_pair) {
    char *token = strtok(key_value_pair, ":");
    if (token != NULL) {
        char *key = strdup(token);
        char *value = strtok(NULL, ":");
        if (value != NULL) {
            if (*(value + 0) == ' ') {
                value++;
            }
            if (strcmp("Combo", key) == 0) {
                if (colour == NULL) {
                    colour = calloc(1, sizeof(Colour));
                } else {
                    colour = realloc(colour, (*num + 1) * sizeof(Colour));
                }
                token = strtok(value, ",");
                enum {
                    red,
                    green,
                    blue
                } rgb = red;
                while (token != NULL) {
                    switch (rgb) {
                        case red:
                            (*colour + *num)->red = (char) strtol(value, NULL, 10);
                            rgb = green;
                            break;

                        case green:
                            (*colour + *num)->green = (char) strtol(value, NULL, 10);
                            rgb = blue;
                            break;

                        case blue:
                            (*colour + *num)->blue = (char) strtol(value, NULL, 10);
                            rgb = red; // Because why not
                            break;
                    }
                    token = strtok(NULL, " ");
                }
                (*num)++;
            }
        }
        free(key);
    }
}

void ofb_colours_free(Colour **colour) {
    if (*colour != NULL) {
        free(*colour);
        *colour = NULL;
    }
}