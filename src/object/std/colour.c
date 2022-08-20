#include "object/std/colour.h"

void oos_colour_free(Colour *colour) {
    if (colour != NULL) {
        free(colour);
        colour = NULL;
    }
}