#include "object/ctb.h"

const int ooc_processor_RNGSEED = 1337;

void ooc_processor_applypositionoffset(CatchHitObject **objects, unsigned int num, bool enabled_hardrock) {
    LegacyRandom rng = ou_legacyrandom_init(ooc_processor_RNGSEED);
    ooc_processor_applypositionoffsetrng(objects, num, &rng, enabled_hardrock);
}

void ooc_processor_applypositionoffsetrng(CatchHitObject **objects, unsigned int num, LegacyRandom *rng, bool enabled_hardrock) {
    float *last_position = NULL;
    double last_start_time = 0;
    
    for (int i = 0; i < num; i++) {
        CatchHitObject *obj = *(objects + i);
        switch (obj->type) {
            case fruit:
                if (enabled_hardrock) {
                    ooc_fruit_applyhardrockoffset(obj, &last_position, &last_start_time, rng);
                }
                break;
            
            case banana_shower:
                ooc_bananashower_xoffset(obj, rng);
                break;

            case juice_stream:
                break;

            case banana:
                break;
        }
    }

    if (last_position != NULL) {
        free(last_position);
    }
}

void ooc_processor_inithyperdash(Difficulty difficulty, CatchHitObject *hit_object, unsigned int num) {
    // TODO
}