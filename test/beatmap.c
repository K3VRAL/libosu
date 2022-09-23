#include <osu.h>

int main(void) {
    Beatmap beatmap;
    of_beatmap_init(&beatmap);
    FILE *fp = fopen("test/beatmap.osu", "r");
    if (fp == NULL) {
        return 1;
    }
    of_beatmap_set(&beatmap, fp);
    fclose(fp);

    for (int i = 0; i < beatmap.num_ho; i++) {
        printf("%d %d %d\n", (beatmap.hit_objects + i)->x, (beatmap.hit_objects + i)->y, (beatmap.hit_objects + i)->time);
    }

    of_beatmap_free(beatmap);

    return 0;
}