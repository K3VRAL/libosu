#include <osu.h>

int main(void) {
    unsigned int object_len = 2;
    CatchHitObject *object = malloc(object_len * sizeof(*object));

    // Banana Shower
    HitObject ho_bs = { .x = 256, .y = 192, .time = 1, .type = spinner, .hit_sound = 0, .ho.spinner.end_time = 262, .hit_sample = {0} };
    ooc_bananashower_init((object + 0), ho_bs);
    ooc_bananashower_createnestedbananas((object + 0));

    // Juice Stream
    HitObject ho_js = { .x = 0, .y = 384, .time = 312, .type = slider, .hit_sound = 0,
        .ho.slider = {
            .curve_type = slidertype_linear,
            .num_curve = 2,
            .curves = NULL,
            .slides = 1,
            .length = 1093.7499666214
        }, .hit_sample = {0}
    };
    ho_js.ho.slider.curves = malloc(ho_js.ho.slider.num_curve * sizeof(*ho_js.ho.slider.curves));
    (ho_js.ho.slider.curves + 0)->x = 512;
    (ho_js.ho.slider.curves + 0)->y = 192;
    (ho_js.ho.slider.curves + 1)->x = 0;
    (ho_js.ho.slider.curves + 1)->y = 0;

    unsigned int tp_len = 2;
    TimingPoint *tp = malloc(tp_len * sizeof(*tp));
    (tp + 0)->time = 0;
    (tp + 0)->beat_length = 200;
    (tp + 0)->meter = 4;
    (tp + 0)->sample_set = 1;
    (tp + 0)->sample_index = 1;
    (tp + 0)->volume = 100;
    (tp + 0)->uninherited = 1;
    (tp + 0)->effects = 0;
    (tp + 1)->time = 0;
    (tp + 1)->beat_length = -50;
    (tp + 1)->meter = 4;
    (tp + 1)->sample_set = 2;
    (tp + 1)->sample_index = 1;
    (tp + 1)->volume = 30;
    (tp + 1)->uninherited = 0;
    (tp + 1)->effects = 0;

    Difficulty difficulty = {
        .slider_multiplier = 3.5,
        .slider_tick_rate = 2
    };

    ooc_juicestream_initwslidertp((object + 1), difficulty, tp, tp_len, ho_js);
    ooc_juicestream_createnestedjuice((object + 1));

    LegacyRandom rng;
    ou_legacyrandom_init(&rng, ooc_processor_RNGSEED);
    ooc_processor_applypositionoffsetrng(object, object_len, &rng, false);

    for (int i = 0; i < object_len; i++) {
        if ((object + i)->type == catchhitobject_bananashower) {
            for (int j = 0; j < (object + i)->cho.bs.num_banana; j++) {
                printf("%d,192,%d,1,0\n", (int) (((object + i)->cho.bs.bananas + j)->x + ((object + i)->cho.bs.bananas + j)->x_offset), (int) ((object + i)->cho.bs.bananas + j)->start_time);
            }
        } else if ((object + i)->type == catchhitobject_juicestream) {
            for (int j = 0; j < (object + i)->cho.js.num_nested; j++) {
                printf("%d,192,%d,1,0\n", (int) (((object + i)->cho.js.nested + j)->x + ((object + i)->cho.js.nested + j)->x_offset), (int) ((object + i)->cho.js.nested + j)->start_time);
            }
        }
    }

    ooc_hitobject_freebulk(object, object_len);
    oos_hitobject_free(ho_js);
    oos_timingpoint_free(tp);

    return 0;
}