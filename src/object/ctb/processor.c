#include "object/ctb.h"

const int ooc_processor_RNGSEED = 1337;

void ooc_processor_applypositionoffset(CatchHitObject *hit_object, unsigned int num, bool enabled_hardrock) {
    LegacyRandom rng = ou_legacyrandom_init(ooc_processor_RNGSEED);
    ooc_processor_applypositionoffsetrng(hit_object, num, rng, enabled_hardrock);
}

void ooc_processor_applypositionoffsetrng(CatchHitObject *hit_object, unsigned int num, LegacyRandom rng, bool enabled_hardrock) {
    float *last_position = NULL;
    double last_start_time = 0;
    
    for (int i = 0; i < num; i++) {
        CatchHitObject obj = *(hit_object + i);
        switch (obj.type) {
            case fruit:
                if (enabled_hardrock) {
                    ooc_fruit_applyhardrockoffset(&obj, last_position, &last_start_time, &rng);
                }
                break;
            
            case banana_shower:
                ooc_bananashower_xoffset(&obj.cho.bs, rng);
                break;

            case juice_stream:
                break;
        }
    }
}

void ooc_processor_inithyperdash(Difficulty difficulty, CatchHitObject *hit_object, unsigned int num) {
    // TODO
}