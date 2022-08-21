#include "file/beatmap/colour.h"

Colour *ofb_colour_addfromstring(char *key_value_pair) {
    char *token = strtok(key_value_pair, ":");
    Colour *colour = NULL;
    if (token != NULL) {
        colour = malloc(sizeof(Colour));
        char *key = strdup(token);
        char *value = strtok(NULL, "\0");
        if (value != NULL) {
            if (*(value + 0) == ' ') {
                value++;
            }
            if (strncmp("Combo", key, strlen("Combo")) == 0) {
                token = strtok(value, ",");
                enum {
                    red,
                    green,
                    blue
                } rgb = red;
                while (token != NULL) {
                    switch (rgb) {
                        case red: {
                            colour->red = (unsigned char) strtol(token, NULL, 10);
                            rgb = green;
                            break;
                        }

                        case green:
                            colour->green = (unsigned char) strtol(token, NULL, 10);
                            rgb = blue;
                            break;

                        case blue:
                            colour->blue = (unsigned char) strtol(token, NULL, 10);
                            rgb = red; // Because why not
                            break;
                    }
                    token = strtok(NULL, ",");
                }
            }
        }
        free(key);
    }
    return colour;
}

char *ofb_colour_tostring(Colour colour, int i) {
    int i_size = ou_comparing_size(i);
    int blue_size = ou_comparing_size(colour.blue);
    int green_size = ou_comparing_size(colour.green);
    int red_size = ou_comparing_size(colour.red);
    int len = strlen("Combo") + i_size + 1 + 1 + 1 + blue_size + 1 + green_size + 1 + red_size + (1 + 1);
    char *output = malloc(len * sizeof(char));
    snprintf(output, len, "Combo%d : %d,%d,%d\n", i, colour.red, colour.green, colour.blue);
    return output;
}