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
    struct {
        int x;
        int y;
        int t;
    } slider_result[7] = {
        {
            .x = 156,
            .y = 157,
            .t = 5130,
        }, {
            .x = 213,
            .y = 123,
            .t = 5287,
        }, {
            .x = 270,
            .y = 89,
            .t = 5444,
        }, {
            .x = 213,
            .y = 123,
            .t = 5601,
        }, {
            .x = 156,
            .y = 157,
            .t = 5758,
        }, {
            .x = 213,
            .y = 123,
            .t = 5916,
        }, {
            .x = 270,
            .y = 89,
            .t = 6073,
        }
    };
    for (int i = 0; i < slider->num_nested; i++) {
        printf("[%d:%d | %d:%d | %d:%d] - %s\n"
            , slider_result[i].x, (slider->nested + i)->x
            , slider_result[i].y, (slider->nested + i)->y
            , slider_result[i].t, (slider->nested + i)->time
            , slider_result[i].x == (slider->nested + i)->x
                && slider_result[i].y == (slider->nested + i)->y
                && slider_result[i].t == (slider->nested + i)->time
                ? "Success" : "Failed");
    }
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