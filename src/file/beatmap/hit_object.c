#include "file/beatmap/hit_object.h"

HitObject ofb_hitobject_addfromstring(char *string) {
    char *copy = strdup(string);
    char *token = strtok(string, ",");
    HitObject hit_object;
    if (token != NULL) {
        hit_object.x = (int) strtol(token, NULL, 10);
        hit_object.y = (int) strtol(strtok(NULL, ","), NULL, 10);
        hit_object.time = (int) strtol(strtok(NULL, ","), NULL, 10);
        hit_object.type = (int) strtol(strtok(NULL, ","), NULL, 10);
        hit_object.hit_sound = (int) strtol(strtok(NULL, ","), NULL, 10);
        if (hit_object.type == 1
            || hit_object.type == 5) {
            hit_object.ho_type = circle;
        } else if (hit_object.type == 2
            || hit_object.type == 6) {
            hit_object.ho_type = slider;
            hit_object.ho.slider.curve_type = strtok(NULL, "|")[0];
            hit_object.ho.slider.curves = calloc(1, sizeof(HOSliderCurve));
            hit_object.ho.slider.num_curve = 0;
            token = strtok(NULL, ":|,");
            bool in_loop = true;
            while (token != NULL && in_loop) {
                char used_delim = *(copy + (token - string + strlen(token)));
                switch (used_delim) {
                    case ',':
                        in_loop = false;
                    case '|':
                        if (hit_object.ho.slider.curves != NULL) {
                            hit_object.ho.slider.curves = realloc(hit_object.ho.slider.curves, (hit_object.ho.slider.num_curve + 1) * sizeof(HOSliderCurve));
                            (hit_object.ho.slider.curves + hit_object.ho.slider.num_curve - 1)->y = (int) strtol(token, NULL, 10);
                        }
                        break;
                    
                    case ':':
                        (hit_object.ho.slider.curves + hit_object.ho.slider.num_curve)->x = (int) strtol(token, NULL, 10);
                        hit_object.ho.slider.num_curve++;
                        break;
                }
                if (in_loop) {
                    token = strtok(NULL, ":|,");
                }
            }
            hit_object.ho.slider.slides = (int) strtol(strtok(NULL, ","), NULL, 10);
            // `length,edgeSounds,edgeSets` can be removed from a slider and still retain the default propertise
            if ((token = strtok(NULL, ",")) != NULL) {
                hit_object.ho.slider.length = strtod(token, NULL);
            }
            if ((token = strtok(NULL, "|,")) != NULL) {
                hit_object.ho.slider.edge_sounds = calloc(1, sizeof(int));
                hit_object.ho.slider.num_edge_sound = 0;
                in_loop = true;
                while (token != NULL && in_loop) {
                    char used_delim = *(copy + (token - string + strlen(token)));
                    switch (used_delim) {
                        case ',':
                            in_loop = false;
                        case '|':
                            if (hit_object.ho.slider.edge_sounds != NULL) {
                                hit_object.ho.slider.edge_sounds = realloc(hit_object.ho.slider.edge_sounds, (hit_object.ho.slider.num_edge_sound + 1) * sizeof(int));
                                *(hit_object.ho.slider.edge_sounds + hit_object.ho.slider.num_edge_sound) = (int) strtol(token, NULL, 10);
                                hit_object.ho.slider.num_edge_sound++;
                            }
                            break;
                    }
                    if (in_loop) {
                        token = strtok(NULL, "|,");
                    }
                }
            }
            if ((token = strtok(NULL, ":|,")) != NULL && *(copy + (token - string + strlen(token))) == ':') {
                hit_object.ho.slider.edge_sets = calloc(1, sizeof(HOSliderEdge));
                hit_object.ho.slider.num_edge_set = 0;
                in_loop = true;
                while (token != NULL && in_loop) {
                    char used_delim = *(copy + (token - string + strlen(token)));
                    switch (used_delim) {
                        case '\0':
                        case ',':
                            in_loop = false;
                        case '|':
                            (hit_object.ho.slider.edge_sets + hit_object.ho.slider.num_edge_set)->additional = (int) strtol(token, NULL, 10);
                            hit_object.ho.slider.num_edge_set++;
                            break;
                        
                        case ':':
                            if (hit_object.ho.slider.edge_sets != NULL) {
                                hit_object.ho.slider.edge_sets = realloc(hit_object.ho.slider.edge_sets, (hit_object.ho.slider.num_edge_set + 1) * sizeof(HOSliderEdge));
                                (hit_object.ho.slider.edge_sets + hit_object.ho.slider.num_edge_set)->normal = (int) strtol(token, NULL, 10);
                            }
                            break;
                    }
                    if (in_loop) {
                        token = strtok(NULL, ":|,");
                    }
                }
            }
        } else if (hit_object.type == 8
            || hit_object.type == 12) {
            hit_object.ho_type = spinner;
            hit_object.ho.spinner.end_time = (int) strtol(strtok(NULL, ","), NULL, 10);
        }
        // If provided string has `hitSample` section (since it can be defaulted)
        if ((token = strtok(NULL, ":")) != NULL) {
            hit_object.hit_sample.normal_set = (int) strtol(token, NULL, 10);
            hit_object.hit_sample.addition_set = (int) strtol(strtok(NULL, ":"), NULL, 10);
            hit_object.hit_sample.index = (int) strtol(strtok(NULL, ":"), NULL, 10);
            hit_object.hit_sample.volume = (int) strtol(strtok(NULL, ":"), NULL, 10);
            if ((token = strtok(NULL, ":")) != NULL) {
                hit_object.hit_sample.filename = strdup(token);
            }
        }
    }
    free(copy);
    return hit_object;
}

char *ofb_hitobject_tostring(HitObject hit_object) {
    int size_x = ou_comparing_size(hit_object.x);
    int size_y = ou_comparing_size(hit_object.y);
    int size_time = ou_comparing_size(hit_object.time);
    int size_type = ou_comparing_size(hit_object.type);
    int size_hitsound = ou_comparing_size(hit_object.hit_sound);
    int size_total = size_x + 1 + size_y + 1 + size_time + 1 + size_type + 1 + size_hitsound;
    bool with_sample = false;
    int size_hitsample = 0;
    if (hit_object.hit_sample.normal_set != 0 ||
        hit_object.hit_sample.addition_set != 0 ||
        hit_object.hit_sample.index != 0 ||
        hit_object.hit_sample.volume != 0 ||
        hit_object.hit_sample.filename != NULL) {
            with_sample = true;
            int size_normalset = ou_comparing_size(hit_object.hit_sample.normal_set);
            int size_additionset = ou_comparing_size(hit_object.hit_sample.addition_set);
            int size_index = ou_comparing_size(hit_object.hit_sample.index);
            int size_volume = ou_comparing_size(hit_object.hit_sample.volume);
            int size_filename = (hit_object.hit_sample.filename != NULL ? strlen(hit_object.hit_sample.filename) : 0);
            size_hitsample = 1 + size_normalset + 1 + size_additionset + 1 + size_index + 1 + size_volume + 1 + size_filename;
    }
    int len = size_total + 1;
    char *output = malloc((len + 1) * sizeof(char));
    snprintf(output, len, "%d,%d,%d,%d,%d",
        hit_object.x,
        hit_object.y,
        hit_object.time,
        hit_object.type,
        hit_object.hit_sound
    );
    switch (hit_object.ho_type) {
        case circle:
            break;

        case slider: {
            int size_slider = 1;
            char *buffer = malloc((size_slider + 1) * sizeof(char));
            
            {
                size_slider += 1 + 1;
                snprintf(buffer, size_slider, ",%c", hit_object.ho.slider.curve_type);
            }
            
            for (int j = 0; j < hit_object.ho.slider.num_curve; j++) {
                int size_slider_x = ou_comparing_size((hit_object.ho.slider.curves + j)->x);
                int size_slider_y = ou_comparing_size((hit_object.ho.slider.curves + j)->y);
                int size_slider_curve = 1 + size_slider_x + 1 + size_slider_y + 1;
                char *buffer_curve = malloc((size_slider_curve + 1) * sizeof(char));
                snprintf(buffer_curve, size_slider_curve, "|%d:%d", (hit_object.ho.slider.curves + j)->x, (hit_object.ho.slider.curves + j)->y);

                size_slider += size_slider_curve;
                buffer = realloc(buffer, (size_slider + 1) * sizeof(char));
                strcat(buffer, buffer_curve);
                free(buffer_curve);
            }

            {
                int size_slider_slides = ou_comparing_size(hit_object.ho.slider.slides);
                int size_slider_length = ou_comparing_size((int) hit_object.ho.slider.length);
                int trailing_zeros = ou_comparing_trailing(hit_object.ho.slider.length, size_slider_length);
                int size_slider_sl = 1 + size_slider_slides + 1 + (size_slider_length + (trailing_zeros != 0 ? 1 : 0) + trailing_zeros + 1);
                char *buffer_sl = malloc((size_slider_sl + 1) * sizeof(char));
                snprintf(buffer_sl, size_slider_sl, ",%d,%.*f", hit_object.ho.slider.slides, trailing_zeros, hit_object.ho.slider.length);

                size_slider += size_slider_sl;
                buffer = realloc(buffer, (size_slider + 1) * sizeof(char));
                strcat(buffer, buffer_sl);
                free(buffer_sl);
            }

            if (hit_object.ho.slider.edge_sounds != NULL || hit_object.ho.slider.num_edge_sound > 0) {
                for (int j = 0; j < hit_object.ho.slider.num_edge_sound; j++) {
                    int size_slider_edge_sound = ou_comparing_size(*(hit_object.ho.slider.edge_sounds + j));
                    int len_edge_sound = 1 + size_slider_edge_sound + 1;
                    char *buffer_edge_sound = malloc((len_edge_sound + 1) * sizeof(char));
                    snprintf(buffer_edge_sound, len_edge_sound, (j != 0 ? "|%d" : ",%d"), *(hit_object.ho.slider.edge_sounds + j));

                    size_slider += len_edge_sound;
                    buffer = realloc(buffer, (size_slider + 1) * sizeof(char));
                    strcat(buffer, buffer_edge_sound);
                    free(buffer_edge_sound);
                }

                if (hit_object.ho.slider.edge_sounds != NULL || hit_object.ho.slider.num_edge_sound > 0) {
                    for (int j = 0; j < hit_object.ho.slider.num_edge_set; j++) {
                        int size_slider_edge_set_normal = ou_comparing_size((hit_object.ho.slider.edge_sets + j)->normal);
                        int size_slider_edge_set_additional = ou_comparing_size((hit_object.ho.slider.edge_sets + j)->additional);
                        int len_edge_set = 1 + size_slider_edge_set_normal + 1 + size_slider_edge_set_additional + 1;
                        char *buffer_edge_set = malloc((len_edge_set + 1) * sizeof(char));
                        snprintf(buffer_edge_set, len_edge_set, (j != 0 ? "|%d:%d" : ",%d:%d"), (hit_object.ho.slider.edge_sets + j)->normal, (hit_object.ho.slider.edge_sets + j)->additional);
                    
                        size_slider += len_edge_set;
                        buffer = realloc(buffer, (size_slider + 1) * sizeof(char));
                        strcat(buffer, buffer_edge_set);
                        free(buffer_edge_set);
                    }
                }
            }

            len += size_slider;
            output = realloc(output, (len + 1) * sizeof(char));
            strcat(output, buffer);
            free(buffer);
            break;
        }

        case spinner: {
            int size_spinner = ou_comparing_size(hit_object.ho.spinner.end_time);
            int len_buffer = 1 + size_spinner;
            char *buffer = malloc((len_buffer + 1) * sizeof(char));
            snprintf(buffer, len_buffer, ",%d", hit_object.ho.spinner.end_time);

            len += len_buffer;
            output = realloc(output, (len + 1) * sizeof(char));
            strcat(output, buffer);
            free(buffer);
            break;
        }
    }

    if (with_sample) {
        int len_buffer = 1 + size_hitsample;
        char *buffer = malloc((len_buffer + 1) * sizeof(char));
        snprintf(buffer, len_buffer, ",%d:%d:%d:%d:%s",
            hit_object.hit_sample.normal_set,
            hit_object.hit_sample.addition_set,
            hit_object.hit_sample.index,
            hit_object.hit_sample.volume,
            hit_object.hit_sample.filename != NULL ? hit_object.hit_sample.filename : ""
        );

        len += len_buffer;
        output = realloc(output, (len + 1) * sizeof(char));
        strcat(output, buffer);
        free(buffer);
    }
    strcat(output, "\n");
    return output;
}