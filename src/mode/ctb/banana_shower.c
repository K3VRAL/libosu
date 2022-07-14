#include "mode/ctb/banana_shower.h"

BananaShower omc_bananashower_init(HitObject hit_object) {
    BananaShower banana_shower = {
        .start = hit_object.time,
        .end = hit_object.ho.spinner.end_time,
        .duration = hit_object.ho.spinner.end_time - hit_object.time,
        .bananas = NULL,
        .num_banana = 0
    };

    double spacing = banana_shower.duration;
    while (spacing > 100) {
        spacing /= 2;
    }

    if (spacing <= 0) {
        return banana_shower;
    }

    double time = banana_shower.start;
    while (time <= banana_shower.end) {
        if (banana_shower.bananas == NULL) {
            banana_shower.bananas = calloc(1, sizeof(Banana));
        } else {
            banana_shower.bananas = realloc(banana_shower.bananas, (banana_shower.num_banana + 1) * sizeof(Banana));
        }
        (banana_shower.bananas + banana_shower.num_banana)->time = time;
        time += spacing;
        banana_shower.num_banana++;
    }

    return banana_shower;
}

void omc_bananashower_free(BananaShower *bs) {
    if (bs->bananas) {
        free(bs->bananas);
        bs->bananas = NULL;
    }
}

LegacyRandom omc_bananashower_xoffset(BananaShower *bs, LegacyRandom lr) {
    return omc_bananashower_xoffsetuntilindex(bs, lr, bs->num_banana);
}

LegacyRandom omc_bananashower_xoffsetuntilindex(BananaShower *bs, LegacyRandom lr, int index) {
    for (int i = 0; i < index; i++) {
        (bs->bananas + i)->x = (float) (legacyrandom_nextdouble(&lr) * omc_playfield_WIDTH);
        legacyrandom_next(&lr);
        legacyrandom_next(&lr);
        legacyrandom_next(&lr);
    }
    return lr;
}