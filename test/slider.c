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
    oos_slider_createnestedhitobjects(slider);

    // TODO make this into it's own function so testing can be easier and more readable
    printf("%s - ", ((int) (slider->nested + 0)->x == 156 && (int) (slider->nested + 0)->y == 157
                    && (int) (slider->nested + 0)->time == 5130) ? "Success" : "Failed");
    printf("%s - ", ((int) (slider->nested + 1)->x == 213 && (int) (slider->nested + 1)->y == 123
                    && (int) (slider->nested + 1)->time == 5287) ? "Success" : "Failed");
    printf("%s - ", ((int) (slider->nested + 2)->x == 270 && (int) (slider->nested + 2)->y == 89
                    && (int) (slider->nested + 2)->time == 5444) ? "Success" : "Failed");
    printf("%s - ", ((int) (slider->nested + 3)->x == 213 && (int) (slider->nested + 3)->y == 123
                    && (int) (slider->nested + 3)->time == 5601) ? "Success" : "Failed");
    printf("%s - ", ((int) (slider->nested + 4)->x == 156 && (int) (slider->nested + 4)->y == 157
                    && (int) (slider->nested + 4)->time == 5758) ? "Success" : "Failed");
    printf("%s - ", ((int) (slider->nested + 5)->x == 213 && (int) (slider->nested + 5)->y == 123
                    && (int) (slider->nested + 5)->time == 5916) ? "Success" : "Failed");
    printf("%s\n", ((int) (slider->nested + 6)->x == 270 && (int) (slider->nested + 6)->y == 89
                    && (int) (slider->nested + 6)->time == 6037) ? "Success" : "Failed");

    oos_slider_free(slider);
    oos_hitobject_free(hit_object, 1);
    oos_uninheritedpoint_free(unintp);
    oos_inheritedpoint_free(intp);
    oos_timingpoint_free(tp);
}