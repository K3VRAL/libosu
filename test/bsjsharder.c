#include <osu.h>

int main(void) {
	unsigned int object_len = 5;
	CatchHitObject *object = malloc(object_len * sizeof(*object));

	Beatmap beatmap = {0};
	of_beatmap_init(&beatmap);
	FILE *fp = fopen("test/beatmap.osu", "r");
	if (fp == NULL) {
		return 1;
	}
	of_beatmap_set(&beatmap, fp);
	fclose(fp);

	for (int i = 0; i < object_len; i++) {
		if (i % 2 == 0) {
			// Banana Shower
			HitObject ho_bs = { .x = 256, .y = 192, .time = i + 10, .type = spinner, .hit_sound = 0, .ho.spinner.end_time = i + 11, .hit_sample = {0} };
			ooc_bananashower_init((object + i), ho_bs);
			ooc_bananashower_createnestedbananas((object + i));
		} else {
			// Juice Stream
			HitObject ho_js = { .x = 0, .y = 384, .time = i + 10, .type = slider, .hit_sound = 0,
				.ho.slider = {
					.curve_type = slidertype_linear, // TODO maybe this `linear` since osu! uses `bezier`
					.num_curve = 1,
					.curves = NULL,
					.slides = 1,
					.length = 300
				}, .hit_sample = {0}
			};
			ho_js.ho.slider.curves = malloc(ho_js.ho.slider.num_curve * sizeof(*ho_js.ho.slider.curves));
			(ho_js.ho.slider.curves + 0)->x = 0;
			(ho_js.ho.slider.curves + 0)->y = 0;
			ooc_juicestream_initwslidertp((object + i), beatmap.difficulty, beatmap.timing_points, beatmap.num_tp, ho_js);
			ooc_juicestream_createnestedjuice((object + i));
		}
	}

	LegacyRandom rng;
	ou_legacyrandom_init(&rng, ooc_processor_RNGSEED);
	ooc_processor_applypositionoffsetrng(object, object_len, &rng, false);
	// printf("//i:%d b:%u w:%u x:%u y:%u z:%u\n", rng.bitIndex, rng.bitBuffer, rng.w, rng.x, rng.y, rng.z);

	for (int i = 0; i < object_len; i++) {
		if ((object + i)->type == catchhitobject_bananashower) {
			printf("256,192,%d,8,0,%d\n", (int) (object + i)->start_time, (object + i)->cho.bs.end_time);
			
			// for (int j = 0; j < (object + i)->cho.bs.num_banana; j++) {
			//     printf("%d,192,%d,5,0\n", (int) (((object + i)->cho.bs.bananas + j)->x + ((object + i)->cho.bs.bananas + j)->x_offset), (int) ((object + i)->cho.bs.bananas + j)->start_time);
			// }
		} else if ((object + i)->type == catchhitobject_juicestream) {
			printf("%d,384,%d,6,0,%c", (int) (object + i)->x, (int) (object + i)->start_time, (object + i)->cho.js.slider_data.ho_data->curve_type);
			for (int j = 0; j < (object + i)->cho.js.slider_data.ho_data->num_curve; j++) {
				printf("|%d:%d", ((object + i)->cho.js.slider_data.ho_data->curves + j)->x, ((object + i)->cho.js.slider_data.ho_data->curves + j)->y);
			}
			printf(",1,%d\n", (int) (object + i)->cho.js.slider_data.path.distance);
			
			/*for (int j = 0; j < (object + i)->cho.js.num_nested; j++) {
				printf("%d,192,%d,5,0\n", (int) (((object + i)->cho.js.nested + j)->x + ((object + i)->cho.js.nested + j)->x_offset), (int) ((object + i)->cho.js.nested + j)->start_time);
			}*/
		}
	}

	ooc_hitobject_freebulk(object, object_len);
	of_beatmap_free(beatmap);

	return 0;
}