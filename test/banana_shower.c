#include <osu.h>

int main(void) {
	Beatmap beatmap = {0};
	of_beatmap_init(&beatmap);
	FILE *fp = fopen("test/test.osu", "r");
	if (fp == NULL) {
		return 1;
	}
	of_beatmap_set(&beatmap, fp);
	fclose(fp);
	
	LegacyRandom rng;
	ou_legacyrandom_init(&rng, ooc_processor_RNGSEED);

	unsigned int banana_shower_len = 2;
	CatchHitObject *banana_shower = malloc(banana_shower_len * sizeof(*banana_shower));
	
	// Juice Stream
	HitObject ho_js = { .x = 256, .y = 384, .time = 1, .type = slider, .hit_sound = 0,
		.ho.slider = {
			.curve_type = slidertype_linear,
			.curves = NULL,
			.num_curve = 0,
			.slides = 1,
			.length = 100
		}, .hit_sample = {0}
	};
	ho_js.ho.slider.curves = realloc(ho_js.ho.slider.curves, ++ho_js.ho.slider.num_curve * sizeof(*ho_js.ho.slider.curves));
	(ho_js.ho.slider.curves + 0)->x = 256;
	(ho_js.ho.slider.curves + 0)->y = 0;
	ooc_juicestream_initwslidertp((banana_shower + 0), *beatmap.difficulty, beatmap.timing_points, beatmap.num_tp, &ho_js);
	ooc_juicestream_createnestedjuice((banana_shower + 0));

	// Banana Shower
	HitObject hit_object = { .x = 256, .y = 192, .time = 1, .type = spinner, .hit_sound = 0, .ho.spinner.end_time = 2, .hit_sample = {0} };
	ooc_bananashower_init((banana_shower + 1), &hit_object);
	ooc_bananashower_createnestedbananas((banana_shower + 1));

	ooc_processor_applypositionoffsetrngwo(banana_shower, banana_shower_len, &rng, false);

	for (int i = 0; i < banana_shower_len; i++) {
		if ((banana_shower + i)->type == catchhitobject_bananashower) {
			for (int j = 0; j < (banana_shower + i)->cho.bs->num_banana; j++) {
				printf("bs %f\n", ((banana_shower + i)->cho.bs->bananas + j)->x + ((banana_shower + i)->cho.bs->bananas + j)->x_offset);
			}
		} else if ((banana_shower + i)->type == catchhitobject_juicestream) {
			for (int j = 0; j < (banana_shower + i)->cho.js->num_nested; j++) {
				printf("js %f\n", ((banana_shower + i)->cho.js->nested + j)->x + ((banana_shower + i)->cho.js->nested + j)->x_offset);
			}
		}
	}

	ooc_hitobject_freebulk(banana_shower, banana_shower_len);
	oos_hitobject_free(ho_js);
	of_beatmap_free(beatmap);

	return 0;
}