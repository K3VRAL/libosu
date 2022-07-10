#include "file/colours.h"

Colours osuc_init() {
    Colours colours = {
        .combo = NULL,
        .num = 0
    };
    return colours;
}

void osuc_free(Colours *c) {
    if (c->combo != NULL) {
        free(c);
    }
}

void osuc_add_C(Colours *colours, char *key_value_pair) {
    char *token = strtok(key_value_pair, ":");
    if (token != NULL) {
        char *key = strdup(token);
        char *value = strdup(strtok(NULL, ":"));
        if (*(value + 0) == ' ') {
            value++;
        }
        if (strcmp("Combo", key) == 0) {
            if (colours->combo == NULL) {
                colours->combo = malloc(sizeof(Combo));
            } else {
                colours->combo = realloc(colours->combo, (colours->num + 1) * sizeof(Combo));
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
                        (colours->combo + colours->num)->red = (char) strtol(value, NULL, 10);
                        rgb = green;
                        break;

                    case green:
                        (colours->combo + colours->num)->green = (char) strtol(value, NULL, 10);
                        rgb = blue;
                        break;

                    case blue:
                        (colours->combo + colours->num)->blue = (char) strtol(value, NULL, 10);
                        rgb = red; // Because why not
                        break;
                }
                token = strtok(NULL, " ");
            }
            colours->num++;
        }
        free(key);
        free(value);
    }
}