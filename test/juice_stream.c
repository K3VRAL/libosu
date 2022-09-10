#include <osu.h>

int main(void) {
    HitObject *hit_object = NULL;
    {
        char *str = strdup("156,157,5130,6,0,L|273:88,3,132.999995941162,2|0|0|0,0:0|0:0|0:0|0:0,0:0:0:0:");
        ofb_hitobject_addfromstring(&hit_object, str);
        free(str);
    }

    TimingPoint *tp = NULL;
    unsigned int tp_len = 0;
    {
        char *str = strdup("99,314.465408805031,4,1,1,100,1,0");
        ofb_timingpoint_addfromstring(&tp, str);
        tp_len++;
        free(str);
    }
    {
        char *str = strdup("99,-142.857142857143,4,2,1,30,0,0");
        
        TimingPoint *temp = NULL;
        ofb_timingpoint_addfromstring(&temp, str);
        tp = realloc(tp, (tp_len + 1) * sizeof(*tp));
        *(tp + tp_len++) = *temp;

        free(str);
        free(temp);
    }

    InheritedTimingPoint *intp = NULL;
    oos_inheritedpoint_init(&intp, tp, tp_len);

    UninheritedTimingPoint *unintp = NULL;
    oos_uninheritedpoint_init(&unintp, tp, tp_len);
    
    Difficulty diff = {
        .slider_multiplier = 1.9,
        .slider_tick_rate = 2
    };

    Slider *slider = NULL;
    oos_slider_init(&slider, diff, intp, unintp, hit_object);
    
    CatchHitObject *object = NULL;
    ooc_juicestream_init(&object, slider);
    ooc_juicestream_createnestedjuice(object);
    ooc_processor_applypositionoffset(&object, 1, false);

    for (int i = 0; i < object->cho.js.num_nested; i++) {
        printf("%d,192,%d,1,0\n",(int) ((object->cho.js.nested + i)->x + (object->cho.js.nested + i)->x_offset), (int) (object->cho.js.nested + i)->start_time);
    }

    ooc_hitobject_free(object);
    oos_slider_free(slider);
    oos_hitobject_free(hit_object);
    oos_uninheritedpoint_free(unintp);
    oos_inheritedpoint_free(intp);
    oos_timingpoint_free(tp);

    return 0;
}