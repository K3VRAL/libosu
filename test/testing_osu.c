#include <stdio.h>
#include <osu.h>
#include <osu/unrelated/reading_line.h>

typedef union ObjectData {
    Structure structure;
    General general;
    Editor editor;
    Metadata metadata;
    Difficulty difficulty;
    Event *event;
    TimingPoint *timing_point;
    Colour *colour;
    HitObject *hit_object;
} ObjectData;
typedef enum ObjectType {
    structure,
    general,
    editor,
    metadata,
    difficulty,
    event,
    timing_point,
    colour,
    hit_object
} ObjectType;
typedef union ObjectSetAdd {
    void (*setfromstring)(ObjectData *, char *);
    ObjectData (*addfromstring)(char *);
} ObjectSetAdd;
typedef struct Object {
    ObjectType type;
    ObjectData data;
    char *read_from_file;
    char *output_to_file;
    ObjectSetAdd setadd;
    char *(*tostring)(ObjectData);
    void (*free)(ObjectData *);
} Object;
void t_object(Object);

void t_beatmap(char *, char *);

int main(int argc, char **argv) {
    if (strcmp("object", *(argv + 1)) == 0) {
        Object object[] = { // TODO check if `(void *)` breaks things
            {
                .type = structure,
                .data.structure = oos_structure_init(),
                .read_from_file = "test/beatmap/object/structure.osu",
                .output_to_file = "bin/test_structure.osu",
                .setadd.setfromstring = (void *) ofb_structure_setfromstring,
                .tostring = (void *) ofb_structure_tostring,
                .free = (void *) oos_structure_free
            }, {
                .type = general,
                .data.general = oos_general_init(),
                .read_from_file = "test/beatmap/object/general.osu",
                .output_to_file = "bin/test_general.osu",
                .setadd.setfromstring = (void *) ofb_general_setfromstring,
                .tostring = (void *) ofb_general_tostring,
                .free = (void *) oos_general_free
            }, {
                .type = editor,
                .data.editor = oos_editor_init(),
                .read_from_file = "test/beatmap/object/editor.osu",
                .output_to_file = "bin/test_editor.osu",
                .setadd.setfromstring = (void *) ofb_editor_setfromstring,
                .tostring = (void *) ofb_editor_tostring,
                .free = (void *) oos_editor_free
            }, {
                .type = metadata,
                .data.metadata = oos_metadata_init(),
                .read_from_file = "test/beatmap/object/metadata.osu",
                .output_to_file = "bin/test_metadata.osu",
                .setadd.setfromstring = (void *) ofb_metadata_setfromstring,
                .tostring = (void *) ofb_metadata_tostring,
                .free = (void *) oos_metadata_free
            }, {
                .type = difficulty,
                .data.difficulty = oos_difficulty_init(),
                .read_from_file = "test/beatmap/object/difficulty.osu",
                .output_to_file = "bin/test_difficulty.osu",
                .setadd.setfromstring = (void *) ofb_difficulty_setfromstring,
                .tostring = (void *) ofb_difficulty_tostring,
                .free = (void *) oos_difficulty_free
            }, {
                .type = event,
                .data.event = NULL,
                .read_from_file = "test/beatmap/object/event.osu",
                .output_to_file = "bin/event.osu",
                .setadd.addfromstring = (void *) ofb_event_addfromstring,
                .tostring = (void *) ofb_event_tostring,
                .free = (void *) oos_event_free
            }, {
                .type = timing_point,
                .data.timing_point = NULL,
                .read_from_file = "test/beatmap/object/timing_point.osu",
                .output_to_file = "bin/test_timing_point.osu",
                .setadd.addfromstring = (void *) ofb_timingpoint_addfromstring,
                .tostring = (void *) ofb_timingpoint_tostring,
                .free = (void *) oos_timingpoint_free
            }, {
                .type = colour,
                .data.colour = NULL,
                .read_from_file = "test/beatmap/object/colour.osu",
                .output_to_file = "bin/test_colour.osu",
                .setadd.addfromstring = (void *) ofb_colour_addfromstring,
                .tostring = (void *) ofb_colour_tostring,
                .free = (void *) oos_colour_free
            }, {
                .type = hit_object,
                .data.hit_object = NULL,
                .read_from_file = "test/beatmap/object/hit_object.osu",
                .output_to_file = "bin/test_hit_object.osu",
                .setadd.setfromstring = (void *) ofb_hitobject_addfromstring,
                .tostring = (void *) ofb_hitobject_tostring,
                .free = (void *) oos_hitobject_free
            }
        };
        for (int i = 0; i < 9; i++) {
            t_object(*(object + i));
        }
    } else if (strcmp("beatmap", *(argv + 1)) == 0) {
        t_beatmap(*(argv + 1), *(argv + 2));
    }
    return 0;
}

void t_object(Object object) {
    {
        FILE *fp_file = fopen(object.read_from_file, "r");
        char *temp;
        while ((temp = ou_readingline_line(fp_file)) != NULL) {
            switch (object.type) {
                // TODO check if `object.data` is correct. We don't want the union, we want the data inside the union
                case structure:
                case general:
                case editor:
                case metadata:
                case difficulty:
                    object.setadd.setfromstring(&object.data, temp);
                    break;

                // TODO lol how to do this?
                // case event:
                // case timing_point:
                // case colour:
                // case hit_object: {
                //     ObjectData *temp;
                //     if ((temp = object.setadd.addfromstring(temp)) == NULL) {
                //         break;
                //     }
                //     object.data = realloc(object.data, (beatmap->num_event + 1) * sizeof(Event));
                //     *(beatmap->events + beatmap->num_event) = *temp;
                //     beatmap->num_event++;
                //     free(temp);
                //     break;
                // }
                default:
                    return;
            }
        }
        fclose(fp_file);
    }

    {
        remove(object.output_to_file);
        FILE *fp_output = fopen(object.output_to_file, "a");
        if (fp_output == NULL) {
            return;
        }
        char *temp = object.tostring(object.data);
        fputs(temp, fp_output);
        free(temp);
        fclose(fp_output);
    }

    object.free(&object.data);
}

void t_beatmap(char *file, char *output) {
    Beatmap beatmap = of_beatmap_init();
    of_beatmap_set(&beatmap, file);

    remove(output);
    FILE *fp = fopen(output, "a");
    if (fp == NULL) {
        return;
    }
    of_beatmap_tofile(&beatmap, fp);

    of_beatmap_free(&beatmap);
    fclose(fp);
}