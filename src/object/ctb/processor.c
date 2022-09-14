#include "object/ctb.h"

const int ooc_processor_RNGSEED = 1337;

void ooc_processor_applypositionoffset(CatchHitObject *objects, unsigned int num, bool enabled_hardrock) {
    LegacyRandom rng;
    ou_legacyrandom_init(&rng, ooc_processor_RNGSEED);
    ooc_processor_applypositionoffsetrng(objects, num, &rng, enabled_hardrock);
}

void ooc_processor_applypositionoffsetrng(CatchHitObject *objects, unsigned int num, LegacyRandom *rng, bool enabled_hardrock) {
    ooc_processor_applypositionoffsetrngstarttime(objects, 0, num, rng, enabled_hardrock);
}

void ooc_processor_applypositionoffsetrngstarttime(CatchHitObject *objects, unsigned int start, unsigned int num, LegacyRandom *rng, bool enabled_hardrock) {
    float *last_position = NULL;
    double last_start_time = 0;
    
    for (int i = start; i < num; i++) {
        switch ((objects + i)->type) {
            case catchhitobject_fruit:
                if (enabled_hardrock) {
                    ooc_fruit_applyhardrockoffset((objects + i), &last_position, &last_start_time, rng);
                }
                break;
            
            case catchhitobject_bananashower:
                ooc_bananashower_xoffset((objects + i), rng);
                break;

            case catchhitobject_juicestream:
                ooc_juicestream_xoffset((objects + i), &last_position, &last_start_time, rng);
                break;

            case catchhitobject_banana:
            case catchhitobject_droplet:
            case catchhitobject_tinydroplet:
                break;
        }
    }

    if (last_position != NULL) {
        free(last_position);
    }
}

void ooc_processor_inithyperdash(Difficulty difficulty, CatchHitObject *hit_object, unsigned int num) {
}