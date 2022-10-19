#include "object/ctb.h"

// TODO implement this in `banana_shower.c`
void ooc_banana_init(CatchHitObject *object, float start_time) {
	ooc_hitobject_init(object, start_time, 0, 0, NULL);
	object->type = catchhitobject_banana;
}