#include "object/ctb.h"

const float ooc_hitobject_OBJECTRADIUS = 64;

void ooc_hitobject_init(CatchHitObject *object, float start_time, float x, float x_offset, HitObject *refer) {
	object->start_time = start_time;
	object->x = x;
	object->x_offset = x_offset;
	object->refer = refer;
}

void ooc_hitobject_free(CatchHitObject object) {
	switch (object.type) {
		case catchhitobject_juicestream:
			ooc_juicestream_free(object.cho.js);
			break;

		case catchhitobject_bananashower:
			ooc_bananashower_free(object.cho.bs);
			break;

		case catchhitobject_fruit:
		case catchhitobject_banana:
		case catchhitobject_droplet:
		case catchhitobject_tinydroplet:
			break;
	}
	if (object.refer != NULL) {
		oos_hitobject_freebulk(object.refer, 1);
	}
}

void ooc_hitobject_freebulk(CatchHitObject *object, unsigned int num) {
	for (int i = 0; i < num; i++) {
		ooc_hitobject_free(*(object + i));
	}
	free(object);
}