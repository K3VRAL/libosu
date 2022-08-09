#include "file/beatmap/colours.h"
#include <stdlib.h>

void ofb_colours_add(Colour **colour, unsigned int *num, Colour col) {
    if (*colour == NULL) {
        // If there is no space; make space
        *colour = calloc(1, sizeof(Colour));
    } else {
        // If there is space; make more space
        *colour = realloc(*colour, (*num + 1) * sizeof(Colour));
    }
    *(*colour + *num) = col;
}

void ofb_colours_add_string(Colour **colour, unsigned int *num, char *key_value_pair) {
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

void ofb_colours_tofile(Colour *colour, unsigned int num, FILE *fp) {
    fputs("[Colours]\n", fp);
    for (int i = 0; i < num; i++) {
        int i_size = (i == 0 ? 1 : (floor(log10(abs(i))) + 1 + (i < 0 ? 1 : 0)));
        int blue_size = ((colour + i)->blue == 0 ? 1 : (floor(log10(abs((int) (colour + num)->blue))) + 1 + ((colour + num)->blue < 0 ? 1 : 0)));
        int green_size = ((colour + i)->green == 0 ? 1 : (floor(log10(abs((int) (colour + num)->green))) + 1 + ((colour + num)->green < 0 ? 1 : 0)));
        int red_size = ((colour + i)->red == 0 ? 1 : (floor(log10(abs((int) (colour + num)->red))) + 1 + ((colour + num)->red < 0 ? 1 : 0)));
        char *output = malloc((strlen("Combo") + i_size + 3 + blue_size + 1 + green_size + 1 + red_size + 2) * sizeof(char));
        sprintf(output, "Combo%d : %d,%d,%d\n", i, (colour + num)->blue, (colour + num)->green, (colour + num)->red);
        fputs(output, fp);
        free(output);
    }
    fputs("\n", fp);
}