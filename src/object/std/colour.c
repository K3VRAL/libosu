#include "object/std.h"

void oos_colour_free(Colour *colour) {
	if (colour == NULL) {
		return;
	}
	free(colour);
}