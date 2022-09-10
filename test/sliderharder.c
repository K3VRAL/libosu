#include <osu.h>

int main(void) {
    HitObject *hit_object = NULL;
    {
        // char *str = strdup("96,192,413,6,0,L|480:192|480:192|0:192|0:192|256:192|256:192|32:192|32:192|512:192,1,1805");
        char *str = strdup("96,192,413,6,0,L|480:192|0:192|256:192|32:192|512:192,1,1805");
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
        char *str = strdup("99,-25.5,4,2,1,30,0,0");
        
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
    oos_slider_createnestedhitobjects(slider);

    for (int i = 0; i < slider->num_nested; i++) {
        printf("%d,%d,%d,1,0\n", (slider->nested + i)->x, (slider->nested + i)->y, (slider->nested + i)->time);
    }

    oos_slider_free(slider);
    oos_hitobject_free(hit_object);
    oos_uninheritedpoint_free(unintp);
    oos_inheritedpoint_free(intp);
    oos_timingpoint_free(tp);

    return 0;
}