#include "object/ctb.h"

const int ooc_processor_RNGSEED = 1337;

void ooc_processor_applypositionoffset(CatchHitObject *objects, unsigned int num, bool enabled_hardrock) {
	LegacyRandom rng;
	ou_legacyrandom_init(&rng, ooc_processor_RNGSEED);
	float *last_position = NULL;
	double last_start_time = 0;
	ooc_processor_applypositionoffsetrng(objects, num, &rng, enabled_hardrock, &last_position, &last_start_time);
	if (last_position != NULL) {
		free(last_position);
	}
}

void ooc_processor_applypositionoffsetrng(CatchHitObject *objects, unsigned int num, LegacyRandom *rng, bool enabled_hardrock, float **last_position, double *last_start_time) {
	ooc_processor_applypositionoffsetrngstarttime(objects, 0, num, rng, enabled_hardrock, last_position, last_start_time);
}

void ooc_processor_applypositionoffsetrngwo(CatchHitObject *objects, unsigned int num, LegacyRandom *rng, bool enabled_hardrock) {
	float *last_position = NULL;
	double last_start_time = 0;
	ooc_processor_applypositionoffsetrngstarttime(objects, 0, num, rng, enabled_hardrock, &last_position, &last_start_time);
	if (last_position != NULL) {
		free(last_position);
	}
}

void ooc_processor_applypositionoffsetrngstarttime(CatchHitObject *objects, unsigned int start, unsigned int num, LegacyRandom *rng, bool enabled_hardrock, float **last_position, double *last_start_time) {
	for (int i = start; i < num; i++) {
		switch ((objects + i)->type) {
			case catchhitobject_fruit:
				if (enabled_hardrock) {
					ooc_fruit_applyhardrockoffset((objects + i), last_position, last_start_time, rng);
				}
				break;
			
			case catchhitobject_bananashower:
				ooc_bananashower_xoffset((objects + i), rng);
				break;

			case catchhitobject_juicestream:
				ooc_juicestream_xoffset((objects + i), last_position, last_start_time, rng);
				break;

			case catchhitobject_banana:
			case catchhitobject_droplet:
			case catchhitobject_tinydroplet:
				break;
		}
	}
}

void ooc_processor_inithyperdash(Difficulty difficulty, CatchHitObject *hit_object, unsigned int num) {
	CatchHitObject **palpable = NULL;
	unsigned int palpable_num = 0;

	for (unsigned int i = 0; i < num; i++) {
		CatchHitObject *obj = (hit_object + i);
		switch (obj->type) {
			case catchhitobject_fruit:
				palpable = realloc(palpable, (++palpable_num) * sizeof(*palpable));
				*(palpable + palpable_num - 1) = obj;
				break;

			case catchhitobject_juicestream: {
				JuiceStream *objs = obj->cho.js;
				for (unsigned int j = 0; j < objs->num_nested; j++) {
					if ((objs->nested + j)->type == catchhitobject_fruit || (objs->nested + j)->type == catchhitobject_droplet) {
						palpable = realloc(palpable, (++palpable_num) * sizeof(*palpable));
						*(palpable + palpable_num - 1) = (objs->nested + j);
					}
				}
				break;
			}

			case catchhitobject_bananashower:
			case catchhitobject_banana:
			case catchhitobject_droplet:
			case catchhitobject_tinydroplet:
				break;
		}
	}

	for (unsigned int i = 0; i < palpable_num - 1; i++) {
		for (unsigned int j = 0; j < palpable_num - i - 1; j++) {
			if ((*(palpable + j))->start_time > (*(palpable + j + 1))->start_time) {
				CatchHitObject *temp = *(palpable + j);
				*(palpable + j) = *(palpable + j + 1);
				*(palpable + j + 1) = temp;
			}
		}
	}
 
	double half_catcher_width = (((float) (106.75f * fabs(1.0f - 0.7f * (difficulty.circle_size - 5) / 5) * 0.8f)) / 2) / 0.8f;

	int last_direction = 0;
	double last_excess = half_catcher_width;
	for (unsigned int i = 0; i < palpable_num - 1; i++) {
		CatchHitObject *curr_obj = *(palpable + i);
		CatchHitObject *next_obj = *(palpable + i + 1);

		int direction = (next_obj->x + next_obj->x_offset) > (curr_obj->x + curr_obj->x_offset) ? 1 : -1;
		double time = next_obj->start_time - curr_obj->start_time - 1000.0f / 60.0f / 4;
		double distance = abs((next_obj->x + next_obj->x_offset) - (curr_obj->x + curr_obj->x_offset)) - (last_direction == direction ? last_excess : half_catcher_width);
		float hyper = (float) (time * 1.0 - distance);
		if (hyper < 0) {
			curr_obj->to_hyper = 0;
			last_excess = half_catcher_width;
		} else {
			curr_obj->to_hyper = hyper;
			double clamp = (hyper < 0 ? 0 : hyper);
			last_excess = clamp > half_catcher_width ? half_catcher_width : clamp;
		}
		last_direction = direction;
	}

	free(palpable);
}
