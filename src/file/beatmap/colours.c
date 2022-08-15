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
    (*num)++;
}

void ofb_colours_add_string(Colour **colour, unsigned int *num, char *key_value_pair) {
    char *token = strtok(key_value_pair, ":");
    if (token != NULL) {
        char *key = strdup(token);
        char *value = strtok(NULL, "\0");
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

void ofb_colours_free(Colour *colour) {
    if (colour != NULL) {
        free(colour);
        colour = NULL;
    }
}

void ofb_colours_tofile(Colour *colour, unsigned int num, FILE *fp) {
    fputs("[Colours]\n", fp);
    for (int i = 0; i < num; i++) {
        int i_size = ou_comparing_size(i);
        int blue_size = ou_comparing_size((colour + i)->blue);
        int green_size = ou_comparing_size((colour + i)->green);
        int red_size = ou_comparing_size((colour + i)->red);
        int len = strlen("Combo") + i_size + 1 + 1 + 1 + blue_size + 1 + green_size + 1 + red_size + 1;
        char *output = malloc((len + 1) * sizeof(char));
        snprintf(output, len, "Combo%d : %d,%d,%d", i, (colour + num)->blue, (colour + num)->green, (colour + num)->red);
        strcat(output, "\n");
        fputs(output, fp);
        free(output);
    }
    fputs("\n", fp);
}