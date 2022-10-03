#include <osu.h>

int main(void) {
    unsigned int len_hit_objects = 10;
    HitObject *hit_objects = calloc(len_hit_objects, sizeof(*hit_objects));

    for (int i = len_hit_objects, j = 0; i > 0; i--, j++) {
        (hit_objects + j)->x = j;
        (hit_objects + j)->time = i;
    }

    for (int i = 0; i < len_hit_objects; i++) {
        printf("%d,192,%d,1,0\n", (hit_objects + i)->x, (hit_objects + i)->time);
    }

    oos_hitobject_freebulk(hit_objects, len_hit_objects);

    return 0;
}