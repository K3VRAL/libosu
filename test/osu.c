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

void t_object(Object object) {
    FILE *fp_file = fopen(object.read_from_file, "r");
    printf("Reading from file [%s]\n", object.read_from_file);
    char *temp_read;
    while ((temp_read = ou_readingline_line(fp_file)) != NULL) {
        switch (object.type) {
            case structure:
                ofb_structure_setfromstring(&object.data.structure, temp_read);
                break;
            case general:
                ofb_general_setfromstring(&object.data.general, temp_read);
                break;
            case editor:
                ofb_editor_setfromstring(&object.data.editor, temp_read);
                break;
            case metadata:
                ofb_metadata_setfromstring(&object.data.metadata, temp_read);
                break;
            case difficulty:
                ofb_difficulty_setfromstring(&object.data.difficulty, temp_read);
                break;

            case event: {
                Event *data;
                if ((data = ofb_event_addfromstring(temp_read)) == NULL) {
                    break;
                }
                object.data.event = realloc(object.data.event, (object.num_data + 1) * sizeof(Event));
                *(object.data.event + object.num_data) = *data;
                object.num_data++;
                free(data);
                break;
            }

            case timing_point: {
                TimingPoint *data;
                if ((data = ofb_timingpoint_addfromstring(temp_read)) == NULL) {
                    break;
                }
                object.data.timing_point = realloc(object.data.timing_point, (object.num_data + 1) * sizeof(TimingPoint));
                *(object.data.timing_point + object.num_data) = *data;
                object.num_data++;
                free(data);
                break;

            }

            case colour: {
                Colour *data;
                if ((data = ofb_colour_addfromstring(temp_read)) == NULL) {
                    break;
                }
                object.data.colour = realloc(object.data.colour, (object.num_data + 1) * sizeof(Colour));
                *(object.data.colour + object.num_data) = *data;
                object.num_data++;
                free(data);
                break;

            }

            case hit_object: {
                HitObject *data;
                if ((data = ofb_hitobject_addfromstring(temp_read)) == NULL) {
                    break;
                }
                object.data.hit_object = realloc(object.data.hit_object, (object.num_data + 1) * sizeof(HitObject));
                *(object.data.hit_object + object.num_data) = *data;
                object.num_data++;
                free(data);
                break;
            }
        }
    }
    fclose(fp_file);

    remove(object.output_to_file);
    FILE *fp_output = fopen(object.output_to_file, "w");
    if (fp_output == NULL) {
        return;
    }
    printf("Outputing to file [%s]\n", object.output_to_file);
    char *temp_output = NULL;
    char *name;
    switch (object.type) {
        case structure:
            name = "[Structure]";
            temp_output = ofb_structure_tostring(object.data.structure);
            break;
        case general:
            name = "[General]";
            temp_output = ofb_general_tostring(object.data.general);
            break;
        case editor:
            name = "[Editor]";
            temp_output = ofb_editor_tostring(object.data.editor);
            break;
        case metadata:
            name = "[Metadata]";
            temp_output = ofb_metadata_tostring(object.data.metadata);
            break;
        case difficulty:
            name = "[Difficulty]";
            temp_output = ofb_difficulty_tostring(object.data.difficulty);
            break;
        case event:
            name = "[Events]";
            for (int i = 0; i < object.num_data; i++) {
                temp_output = ofb_event_tostring(*(object.data.event + i));
            }
            break;
        case timing_point:
            name = "[TimingPoints]";
            for (int i = 0; i < object.num_data; i++) {
                temp_output = ofb_timingpoint_tostring(*(object.data.timing_point + i));
            }
            break;
        case colour:
            name = "[Colours]";
            for (int i = 0; i < object.num_data; i++) {
                temp_output = ofb_colour_tostring(*(object.data.colour + i), i);
            }
            break;
        case hit_object:
            name = "[HitObjects]";
            for (int i = 0; i < object.num_data; i++) {
                temp_output = ofb_hitobject_tostring(*(object.data.hit_object + i));
            }
            break;
    }
    if (temp_output != NULL) {
        fputs(temp_output, fp_output);
        free(temp_output);
    }
    fclose(fp_output);


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

void t_object_slider() {
    HitObject *hit_object;
    {
        char *str = strdup("156,157,5130,6,0,P|178:96|236:68,1,132.999995941162,2|0,0:0|0:0,0:0:0:0:");
        hit_object = ofb_hitobject_addfromstring(str);
        free(str);
    }

    InheritedTimingPoint *inherited;
    {
        char *str = strdup("99,-142.857142857143,4,2,1,30,0,0");
        TimingPoint *temp = ofb_timingpoint_addfromstring(str);
        inherited = oos_inheritedpoint_init(temp, 1);
        free(temp);
        free(str);
    }

    UninheritedTimingPoint *uninherited;
    {
        char *str = strdup("99,314.465408805031,4,1,1,100,1,0");
        TimingPoint *temp = ofb_timingpoint_addfromstring(str);
        uninherited = oos_uninheritedpoint_init(temp, 1);
        free(temp);
        free(str);
    }

    Difficulty difficulty = {
        .slider_multiplier = 1.9,
        .slider_tick_rate = 2
    };
    Slider *slider = oos_slider_init(difficulty, inherited, uninherited, *hit_object);
    if (slider == NULL) {
        return;
    }
    oos_slider_createnestedhitobjects(slider);
    // TODO output to file

    oos_slider_free(slider);
    oos_hitobject_free(hit_object, 1);
    oos_inheritedpoint_free(inherited);
    oos_uninheritedpoint_free(uninherited);
}

void t_object_fruithardrock() {
    unsigned int num = 13;
    CatchHitObject **objects = malloc(num * sizeof(CatchHitObject));
    for (int i = 0; i < num; i++) {
        HitObject hit_object = {
            .x = 256,
            .y = 192,
            .time = (i * 157) + 99,
            .type = circle,
            .hit_sound = 0,
            .hit_sample = {0}
        };
        printf("x: %d | y: %d time: %d\n", hit_object.x, hit_object.y, hit_object.time);
        *(objects + i) = ooc_fruit_init(hit_object);
    }

    ooc_processor_applypositionoffset(objects, num, true);
    // TODO output to file

    free(objects);
}

void t_object_bananashower() {
    char *str = strdup("256,192,0,12,0,1000");
    HitObject *hit_object = ofb_hitobject_addfromstring(str);
    free(str);
    if (hit_object == NULL) {
        return;
    }
    CatchHitObject *object = ooc_bananashower_init(*hit_object);
    oos_hitobject_free(hit_object, 1);
    ooc_bananashower_createnestedbananas(object);

    ooc_processor_applypositionoffset(&object, 1, false);
    // TODO output to file

    ooc_bananashower_free(&object->cho.bs);
    free(object);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        return 1;
    }
    if (strcmp("object", *(argv + 1)) == 0) {
        // TODO populate files
        Object object[] = {
            {
                .type = structure,
                .data.structure = oos_structure_init(),
                .read_from_file = "test/beatmap/object/structure.osu",
                .output_to_file = "bin/test_object_structure.osu",
            }, {
                .type = general,
                .data.general = oos_general_init(),
                .read_from_file = "test/beatmap/object/general.osu",
                .output_to_file = "bin/test_object_general.osu",
            }, {
                .type = editor,
                .data.editor = oos_editor_init(),
                .read_from_file = "test/beatmap/object/editor.osu",
                .output_to_file = "bin/test_object_editor.osu",
            }, {
                .type = metadata,
                .data.metadata = oos_metadata_init(),
                .read_from_file = "test/beatmap/object/metadata.osu",
                .output_to_file = "bin/test_object_metadata.osu",
            }, {
                .type = difficulty,
                .data.difficulty = oos_difficulty_init(),
                .read_from_file = "test/beatmap/object/difficulty.osu",
                .output_to_file = "bin/test_object_difficulty.osu",
            }, {
                .type = event,
                .data.event = NULL,
                .num_data = 0,
                .read_from_file = "test/beatmap/object/event.osu",
                .output_to_file = "bin/test_object_event.osu",
            }, {
                .type = timing_point,
                .data.timing_point = NULL,
                .num_data = 0,
                .read_from_file = "test/beatmap/object/timing_point.osu",
                .output_to_file = "bin/test_object_timingpoint.osu",
            }, {
                .type = colour,
                .data.colour = NULL,
                .num_data = 0,
                .read_from_file = "test/beatmap/object/colour.osu",
                .output_to_file = "bin/test_object_colour.osu",
            }, {
                .type = hit_object,
                .data.hit_object = NULL,
                .num_data = 0,
                .read_from_file = "test/beatmap/object/hit_object.osu",
                .output_to_file = "bin/test_object_hitobject.osu",
            }
        };
        for (int i = 0; i < 9; i++) {
            t_object(*(object + i));
        }
    } else if (strcmp("beatmap_1", *(argv + 1)) == 0) {
        t_beatmap("test/beatmap/nekodex - new beginnings (pishifat) [osu testing].osu", "bin/test_beatmap1.osu");
    } else if (strcmp("beatmap_2", *(argv + 1)) == 0) {
        t_beatmap("test/beatmap/LeaF - Aleph-0 (Enjuxx) [NULL].osu", "bin/test_beatmap2.osu");
    } else if (strcmp("slider", *(argv + 1)) == 0) {
        t_object_slider();
    } else if (strcmp("fruit_hr", *(argv + 1)) == 0) {
        t_object_fruithardrock();
    } else if (strcmp("banana_shower", *(argv + 1)) == 0) { // TODO fix errors
        t_object_bananashower();
    }
    return 0;
}