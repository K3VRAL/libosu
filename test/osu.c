#include <stdio.h>

#include <osu.h>

void t_of_beatmap_bananashower(char *file) {
    Beatmap beatmap = of_beatmap_init();
    of_beatmap_set(&beatmap, file);
    LegacyRandom lr = ou_legacyrandom_init(omc_processor_RNGSEED);
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
}

void t_of_beatmap_tofile(char *file, char *output) {
    FILE *fp = fopen(output, "a");
    if (fp != NULL) {
        Beatmap beatmap = of_beatmap_init();
        of_beatmap_set(&beatmap, file);
        of_beatmap_tofile(beatmap, fp);
        fclose(fp);
        of_beatmap_free(&beatmap);
    }
}

int main(int argc, char **argv) {
    if (argc == 2) {
        t_of_beatmap_bananashower(argv[1]);
    } else if (argc == 3) {
        t_of_beatmap_tofile(argv[1], argv[2]);
    }
}
