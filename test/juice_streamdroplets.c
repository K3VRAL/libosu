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

	// Juice Stream
	HitObject ho_js = { .x = 256, .y = 384, .time = 1, .type = slider, .hit_sound = 0,
		.ho.slider = {
			.curve_type = slidertype_linear,
			.curves = NULL,
			.num_curve = 0,
			.slides = 1,
			.length = 307
		}, .hit_sample = {0}
	};
	ho_js.ho.slider.curves = realloc(ho_js.ho.slider.curves, ++ho_js.ho.slider.num_curve * sizeof(*ho_js.ho.slider.curves));
	(ho_js.ho.slider.curves + 0)->x = 256;
	(ho_js.ho.slider.curves + 0)->y = 0;
	CatchHitObject object_js;
	ooc_juicestream_initwslidertp(&object_js, beatmap.difficulty, beatmap.timing_points, beatmap.num_tp, &ho_js);
	ooc_juicestream_createnestedjuice(&object_js);

	ooc_processor_applypositionoffsetrng(&object_js, 1, &rng, false);

	for (int j = 0; j < object_js.cho.js.num_nested; j++) {
		printf("js %f\n", (object_js.cho.js.nested + j)->x + (object_js.cho.js.nested + j)->x_offset);
	}

	ooc_hitobject_free(object_js);
	oos_hitobject_free(ho_js);
	of_beatmap_free(beatmap);

	return 0;
}