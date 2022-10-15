#include <osu.h>

int main(void) {
	HitObject hit_object;
	{
		char *str = strdup("151,196,15,6,0,B|688:116|356:79|198:54|197:54,1,525");
		ofb_hitobject_addfromstring(&hit_object, str);
		free(str);
	}

	TimingPoint *tp = malloc(2 * sizeof(*tp));
	unsigned int tp_len = 0;
	{
		char *str = strdup("0,60,4,1,0,5,1,0");
		ofb_timingpoint_addfromstring((tp + tp_len++), str);
		free(str);
	}
	{
		char *str = strdup("0,-100,4,1,0,5,0,0");
		ofb_timingpoint_addfromstring((tp + tp_len++), str);
		free(str);
	}
	
	Difficulty diff = {
		.slider_multiplier = 1,
		.slider_tick_rate = 1
	};

	CatchHitObject object;
	ooc_juicestream_initwslidertp(&object, diff, tp, tp_len, hit_object);
	ooc_juicestream_createnestedjuice(&object);
	ooc_processor_applypositionoffset(&object, 1, false);

	for (int i = 0; i < object.cho.js.num_nested; i++) {
		printf("%d,192,%d,1,0\n", (int) ((object.cho.js.nested + i)->x + (object.cho.js.nested + i)->x_offset), (int) (object.cho.js.nested + i)->start_time);
	}

	ooc_hitobject_free(object);
	oos_hitobject_free(hit_object);
	oos_timingpoint_free(tp);

	return 0;
}