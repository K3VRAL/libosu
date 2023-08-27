#include <osu.h>

int main(void) {
	HitObject hit_object;
	{
		char *str = strdup("156,157,5130,6,0,L|273:88,3,132.999995941162,2|0|0|0,0:0|0:0|0:0|0:0,0:0:0:0:");
		ofb_hitobject_addfromstring(&hit_object, str);
		free(str);
	}

	TimingPoint *tp = malloc(2 * sizeof(*tp));
	unsigned int tp_len = 0;
	{
		char *str = strdup("99,314.465408805031,4,1,1,100,1,0");
		ofb_timingpoint_addfromstring((tp + tp_len++), str);
		free(str);
	}
	{
		char *str = strdup("99,-142.857142857143,4,2,1,30,0,0");
		ofb_timingpoint_addfromstring((tp + tp_len++), str);
		free(str);
	}
	
	Difficulty diff = {
		.slider_multiplier = 1.9,
		.slider_tick_rate = 2
	};

	Slider slider;
	oos_slider_initwtp(&slider, diff, tp, tp_len, hit_object);
	oos_slider_createnestedhitobjects(&slider);

	for (int i = 0; i < slider.num_nested; i++) {
		printf("%d,192,%d,1,0\n", (int) (slider.nested + i)->x, (int) (slider.nested + i)->time);
	}

	oos_slider_free(slider);
	oos_hitobject_free(hit_object);
	oos_timingpoint_free(tp);

	return 0;
}