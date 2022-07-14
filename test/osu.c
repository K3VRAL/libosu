#include <osu.h>
#include "global.h"

#define t_of_beatmap_init_EXCEPTION (1)
void t_of_beatmap_init();

int main(void) {
    TRY {
        t_of_beatmap_init();
    } CATCH (t_of_beatmap_init_EXCEPTION) {
        printf("Exception Caught %d\n", t_of_beatmap_init_EXCEPTION);
    } ETRY;
}

void t_of_beatmap_init() {
    Beatmap beatmap = of_beatmap_init();

    of_beatmap_set(&beatmap, "./test/nekodex - new beginnings (pishifat) [osu testing].osu");
    if (beatmap.metadata.artist == NULL || strcmp("nekodex", beatmap.metadata.artist) != 0) {
        THROW;
    }

    LegacyRandom lr = legacyrandom_init(omc_processor_RNGSEED);
    BananaShower bs;
    for (int i = 0; i < beatmap.num_ho; i++) {
        if ((beatmap.hit_objects + i)->ho_type == spinner) {
            bs = omc_bananashower_init(*(beatmap.hit_objects + i));
            break;
        }
    }
    lr = omc_bananashower_xoffset(&bs, lr);
    omc_bananashower_free(&bs);

    of_beatmap_free(&beatmap);
    if (beatmap.hit_objects != NULL) {
        THROW;
    }
}