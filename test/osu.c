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
typedef struct Object {
    ObjectType type;
    ObjectData data;
    unsigned int num_data;
    char *read_from_file;
    char *output_to_file;
} Object;
void t_object(Object);

void t_beatmap(char *, char *);

int main(int argc, char **argv) {
    if (strcmp("object", *(argv + 1)) == 0) {
        Object object[] = {
            {
                .type = structure,
                .data.structure = oos_structure_init(),
                .read_from_file = "test/beatmap/object/structure.osu",
                .output_to_file = "bin/test_structure.osu",
            }, {
                .type = general,
                .data.general = oos_general_init(),
                .read_from_file = "test/beatmap/object/general.osu",
                .output_to_file = "bin/test_general.osu",
            }, {
                .type = editor,
                .data.editor = oos_editor_init(),
                .read_from_file = "test/beatmap/object/editor.osu",
                .output_to_file = "bin/test_editor.osu",
            }, {
                .type = metadata,
                .data.metadata = oos_metadata_init(),
                .read_from_file = "test/beatmap/object/metadata.osu",
                .output_to_file = "bin/test_metadata.osu",
            }, {
                .type = difficulty,
                .data.difficulty = oos_difficulty_init(),
                .read_from_file = "test/beatmap/object/difficulty.osu",
                .output_to_file = "bin/test_difficulty.osu",
            }, {
                .type = event,
                .data.event = NULL,
                .num_data = 0,
                .read_from_file = "test/beatmap/object/event.osu",
                .output_to_file = "bin/event.osu",
            }, {
                .type = timing_point,
                .data.timing_point = NULL,
                .num_data = 0,
                .read_from_file = "test/beatmap/object/timing_point.osu",
                .output_to_file = "bin/test_timing_point.osu",
            }, {
                .type = colour,
                .data.colour = NULL,
                .num_data = 0,
                .read_from_file = "test/beatmap/object/colour.osu",
                .output_to_file = "bin/test_colour.osu",
            }, {
                .type = hit_object,
                .data.hit_object = NULL,
                .num_data = 0,
                .read_from_file = "test/beatmap/object/hit_object.osu",
                .output_to_file = "bin/test_hit_object.osu",
            }
        };
        for (int i = 0; i < 9; i++) {
            t_object(*(object + i));
        }
    } else if (strcmp("beatmap", *(argv + 1)) == 0) {
        t_beatmap("test/beatmap/object/LeaF - Aleph-0 (Enjuxx) [NULL].osu", "bin/testing.osu");
    }
    return 0;
}

void t_object(Object object) {
    {
        FILE *fp_file = fopen(object.read_from_file, "r");
        printf("Reading from file [%s]\n", object.read_from_file);
        char *temp;
        while ((temp = ou_readingline_line(fp_file)) != NULL) {
            switch (object.type) {
                case structure:
                    ofb_structure_setfromstring(&object.data.structure, temp);
                    break;
                case general:
                    ofb_general_setfromstring(&object.data.general, temp);
                    break;
                case editor:
                    ofb_editor_setfromstring(&object.data.editor, temp);
                    break;
                case metadata:
                    ofb_metadata_setfromstring(&object.data.metadata, temp);
                    break;
                case difficulty:
                    ofb_difficulty_setfromstring(&object.data.difficulty, temp);
                    break;

                // TODO
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
        if (temp) {
            free(temp);
        }
    }

    {
        remove(object.output_to_file);
        FILE *fp_output = fopen(object.output_to_file, "w");
        if (fp_output == NULL) {
            return;
        }
        printf("Outputing to file [%s]\n", object.output_to_file);
        char *temp = NULL, *name;
        switch (object.type) {
            case structure:
                name = "[Structure]";
                temp = ofb_structure_tostring(object.data.structure);
                break;
            case general:
                name = "[General]";
                temp = ofb_general_tostring(object.data.general);
                break;
            case editor:
                name = "[Editor]";
                temp = ofb_editor_tostring(object.data.editor);
                break;
            case metadata:
                name = "[Metadata]";
                temp = ofb_metadata_tostring(object.data.metadata);
                break;
            case difficulty:
                name = "[Difficulty]";
                temp = ofb_difficulty_tostring(object.data.difficulty);
                break;
            case event:
                name = "[Events]";
                for (int i = 0; i < object.num_data; i++) {
                    temp = ofb_event_tostring(*(object.data.event + i));
                }
                break;
            case timing_point:
                name = "[TimingPoints]";
                for (int i = 0; i < object.num_data; i++) {
                    temp = ofb_timingpoint_tostring(*(object.data.timing_point + i));
                }
                break;
            case colour:
                name = "[Colours]";
                for (int i = 0; i < object.num_data; i++) {
                    temp = ofb_colour_tostring(*(object.data.colour + i), i);
                }
                break;
            case hit_object:
                name = "[HitObjects]";
                for (int i = 0; i < object.num_data; i++) {
                    temp = ofb_hitobject_tostring(*(object.data.hit_object + i));
                }
                break;
        }
        if (temp != NULL) {
            fputs(temp, fp_output);
            free(temp);
        }
        fclose(fp_output);
    }


    switch (object.type) {
        case structure:
            oos_structure_free(object.data.structure);
            break;
        case general:
            oos_general_free(object.data.general);
            break;
        case editor:
            oos_editor_free(object.data.editor);
            break;
        case metadata:
            oos_metadata_free(object.data.metadata);
            break;
        case difficulty:
            oos_difficulty_free(object.data.difficulty);
            break;
        case event:
            oos_event_free(object.data.event, object.num_data);
            break;
        case timing_point:
            oos_timingpoint_free(object.data.timing_point);
            break;
        case colour:
            oos_colour_free(object.data.colour);
            break;
        case hit_object:
            oos_hitobject_free(object.data.hit_object, object.num_data);
            break;
    }
}

void t_beatmap(char *file, char *output) {
    Beatmap beatmap = of_beatmap_init();
    of_beatmap_set(&beatmap, file);

    remove(output);
    FILE *fp = fopen(output, "a");
    if (fp == NULL) {
        return;
    }
    of_beatmap_tofile(beatmap, fp);

    of_beatmap_free(&beatmap);
    fclose(fp);
}