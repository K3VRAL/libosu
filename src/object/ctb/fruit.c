#include "object/ctb.h"

const float ooc_fruit_OBJECTRADIUS = 64;

void ooc_fruit_init(CatchHitObject *object, HitObject *hit_object) {
	if (!(hit_object->type == circle || hit_object->type == nc_circle)) {
		return;
	}

	ooc_hitobject_init(object, hit_object->time, hit_object->x, 0, hit_object);
	object->type = catchhitobject_fruit;
}

void ooc_fruit_applyhardrockoffset(CatchHitObject *object, float **last_position, double *last_start_time, LegacyRandom *rng) {
	float offset_position = object->x;
	double start_time = object->start_time;
	if (*last_position == NULL) {
		*last_position = calloc(1, sizeof(**last_position));
		**last_position = offset_position;
		*last_start_time = start_time;
		return;
	}
	float position_diff = offset_position - **last_position;
	int time_diff = (int) (start_time - *last_start_time);
	if (time_diff > 1000) {
		**last_position = offset_position;
		*last_start_time = start_time;
	}
	if (position_diff == 0) {
		ooc_fruit_applyrandomoffset(&offset_position, time_diff / (double) 4, rng);
		object->x_offset = offset_position - object->x;
		return;
	}
	if (fabsf(position_diff) < (float) time_diff / 3) {
		ooc_fruit_applyoffset(&offset_position, position_diff);
	}
	object->x_offset = offset_position - object->x;
	**last_position = offset_position;
	*last_start_time = start_time;
}

void ooc_fruit_applyrandomoffset(float *position, double max_offset, LegacyRandom *rng) {
	bool right = ou_legacyrandom_nextbool(rng);
	float rand = fmin(20, (float) ou_legacyrandom_nextlowerupper(rng, 0, fmax(0, max_offset)));
	if (right) {
		if (*position + rand <= ooc_playfield_WIDTH) {
			*position += rand;
		} else {
			*position -= rand;
		}
	} else {
		if (*position - rand >= 0) {
			*position -= rand;
		} else {
			*position += rand;
		}
	}
}

void ooc_fruit_applyoffset(float *position, float amount) {
	if (amount > 0) {
		if (*position + amount < ooc_playfield_WIDTH) {
			*position += amount;
		}
	} else {
		if (*position + amount > 0) {
			*position += amount;
		}
	}
}