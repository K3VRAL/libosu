#include "object/ctb.h"

void ooc_juicestream_init(CatchHitObject *object, HitObject *hit_object) {
	ooc_hitobject_init(object, hit_object->time, hit_object->x, 0, hit_object);
	object->type = catchhitobject_juicestream;
	object->x_offset = 0;
	object->cho.js.nested = NULL;
	object->cho.js.num_nested = 0;
}

void ooc_juicestream_initwslidertp(CatchHitObject *object, Difficulty difficulty, TimingPoint *timing_point, unsigned int timingpoint_len, HitObject *hit_object) {
	ooc_juicestream_init(object, hit_object);
	oos_slider_initwtp(&object->cho.js.slider_data, difficulty, timing_point, timingpoint_len, *hit_object);
}

void ooc_juicestream_initwsliderherit(CatchHitObject *object, Difficulty difficulty, InheritedTimingPoint inherit, UninheritedTimingPoint uninherit, HitObject *hit_object) {
	ooc_juicestream_init(object, hit_object);
	oos_slider_initwherited(&object->cho.js.slider_data, difficulty, inherit, uninherit, *hit_object);
}

void ooc_juicestream_initwsliderspecific(CatchHitObject *object, Difficulty difficulty, TimingPoint inherit, TimingPoint uninherit, HitObject *hit_object) {
	ooc_juicestream_init(object, hit_object);
	oos_slider_init(&object->cho.js.slider_data, difficulty, inherit, uninherit, *hit_object);
}

void ooc_juicestream_free(JuiceStream juice_stream) {
	if (juice_stream.nested != NULL) {
		free(juice_stream.nested);
		juice_stream.num_nested = 0;
	}
	oos_slider_free(juice_stream.slider_data);
}

// https://github.com/ppy/osu/blob/master/osu.Game.Rulesets.Catch/Beatmaps/CatchBeatmapProcessor.cs
// https://github.com/ppy/osu/blob/master/osu.Game.Rulesets.Catch/Objects/JuiceStream.cs
// https://github.com/ppy/osu/blob/38064165413eb919b9a32433d8be5051fc1215f7/osu.Game/Rulesets/Objects/SliderEventGenerator.cs
void ooc_juicestream_createnestedjuice(CatchHitObject *object) {
	SliderEventDescriptor *last_event = NULL;
	SliderEventDescriptor *e;
	while ((e = oos_slider_generate(object->start_time, object->cho.js.slider_data.span_duration, object->cho.js.slider_data.velocity, object->cho.js.slider_data.tick_distance, object->cho.js.slider_data.path.distance, object->cho.js.slider_data.span_count, object->cho.js.slider_data.legacy_last_tick_offset)) != NULL) {
		if (last_event != NULL) {
			int since_last_tick = (int)(e->time - last_event->time);
			if (since_last_tick > 80) {
				double time_between_tiny = since_last_tick;
				while (time_between_tiny > 100) {
					time_between_tiny /= 2;
				}
				for (double t = time_between_tiny; t < since_last_tick; t += time_between_tiny) {
					SliderVector2 position;
					oos_slider_positionat(&position, (last_event->path_progress + (t / since_last_tick) * (e->path_progress - last_event->path_progress)), &object->cho.js.slider_data);
					object->cho.js.nested = realloc(object->cho.js.nested, (object->cho.js.num_nested + 1) * sizeof(*object->cho.js.nested));
					(object->cho.js.nested + object->cho.js.num_nested)->type = catchhitobject_tinydroplet;
					(object->cho.js.nested + object->cho.js.num_nested)->start_time = t + last_event->time;
					(object->cho.js.nested + object->cho.js.num_nested++)->x = object->x + position.x;
				}
			}
		}
		if (last_event == NULL) {
			last_event = calloc(1, sizeof(*last_event));
		}
		last_event->type = e->type;
		last_event->time = e->time;
		last_event->span_index = e->span_index;
		last_event->span_start_time = e->span_start_time;
		last_event->path_progress = e->path_progress;
		switch (e->type) {
			case sliderevent_tick: {
				SliderVector2 position;
				oos_slider_positionat(&position, e->path_progress, &object->cho.js.slider_data);
				object->cho.js.nested = realloc(object->cho.js.nested, (object->cho.js.num_nested + 1) * sizeof(*object->cho.js.nested));
				(object->cho.js.nested + object->cho.js.num_nested)->type = catchhitobject_droplet;
				(object->cho.js.nested + object->cho.js.num_nested)->start_time = e->time;
				(object->cho.js.nested + object->cho.js.num_nested++)->x = object->x + position.x;
				break;
			}

			case sliderevent_head:
			case sliderevent_tail:
			case sliderevent_repeat: {
				SliderVector2 position;
				oos_slider_positionat(&position, e->path_progress, &object->cho.js.slider_data);
				object->cho.js.nested = realloc(object->cho.js.nested, (object->cho.js.num_nested + 1) * sizeof(*object->cho.js.nested));
				(object->cho.js.nested + object->cho.js.num_nested)->type = catchhitobject_fruit;
				(object->cho.js.nested + object->cho.js.num_nested)->start_time = e->time;
				(object->cho.js.nested + object->cho.js.num_nested++)->x = object->x + position.x;
				break;
			}

			case sliderevent_legacylasttick:
				break;
		}
	}
	free(last_event);
}

void ooc_juicestream_xoffset(CatchHitObject *object, float **last_position, double *last_start_time, LegacyRandom *rng) {
	if (*last_position == NULL) {
		*last_position = calloc(1, sizeof(*last_position));
	}
	**last_position = object->x + (object->cho.js.slider_data.controlpoint_len > 0 ? (object->cho.js.slider_data.control_point + object->cho.js.slider_data.controlpoint_len - 1)->x : 0);
	*last_start_time = object->start_time;
	for (int i = 0; i < object->cho.js.num_nested; i++) {
		(object->cho.js.nested + i)->x_offset = 0;
		if ((object->cho.js.nested + i)->type == catchhitobject_tinydroplet) {
			(object->cho.js.nested + i)->x_offset = fmax(-(object->cho.js.nested + i)->x, fmin(ou_legacyrandom_nextlowerupper(rng, -20, 20), ooc_playfield_WIDTH - (object->cho.js.nested + i)->x));
		} else if ((object->cho.js.nested + i)->type == catchhitobject_droplet) {
			ou_legacyrandom_next(rng);
		}
	}
}