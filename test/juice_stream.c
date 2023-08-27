#include <osu.h>

int main(void) {
	Beatmap beatmap;
	of_beatmap_init(&beatmap);
	FILE *fp = fopen("test/juice_stream.osu", "r");
	if (fp == NULL) {
		return 1;
	}
	of_beatmap_set(&beatmap, fp);
	fclose(fp);

	
	Slider slider;
	oos_slider_initwtp(&slider, *beatmap.difficulty, beatmap.timing_points, beatmap.num_tp, *(beatmap.hit_objects + 0));
	oos_slider_createnestedhitobjects(&slider);

	of_beatmap_free(beatmap);
	oos_slider_free(slider);

	return 0;
}