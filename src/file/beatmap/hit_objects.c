#include "file/beatmap/hit_objects.h"
#include <stdio.h>

void ofb_hitobjects_add(HitObject **ho, unsigned int *num, HitObject hit_objects) {
    if (*ho == NULL) {
        // If there is no space; make space
        *ho = calloc(1, sizeof(HitObject));
    } else {
        // If there is space; make more space
        *ho = realloc(*ho, (*num + 1) * sizeof(HitObject));
    }
    *(*ho + *num) = hit_objects;
    (*num)++;
}

void ofb_hitobjects_add_string(HitObject **ho, unsigned int *num, char *string) {
    char *copy = strdup(string);
    char *token = strtok(string, ",");
    if (token != NULL) {
        if (*ho == NULL) {
            // If there is no space; make space
            *ho = calloc(1, sizeof(HitObject));
        } else {
            // If there is space; make more space
            *ho = realloc(*ho, (*num + 1) * sizeof(HitObject));
        }
        // Set data
        (*ho + *num)->x = (int) strtol(token, NULL, 10);
        (*ho + *num)->y = (int) strtol(strtok(NULL, ","), NULL, 10);
        (*ho + *num)->time = (int) strtol(strtok(NULL, ","), NULL, 10);
        (*ho + *num)->type = (int) strtol(strtok(NULL, ","), NULL, 10);
        (*ho + *num)->hit_sound = (int) strtol(strtok(NULL, ","), NULL, 10);
        if ((*ho + *num)->type == 1
            || (*ho + *num)->type == 5) {
            (*ho + *num)->ho_type = circle;
        } else if ((*ho + *num)->type == 2
            || (*ho + *num)->type == 6) {
            (*ho + *num)->ho_type = slider;
            (*ho + *num)->ho.slider.curve_type = strtok(NULL, "|")[0];
            (*ho + *num)->ho.slider.curves = calloc(1, sizeof(HOSliderCurve));
            (*ho + *num)->ho.slider.num_curve = 0;
            token = strtok(NULL, ":|,");
            bool in_loop = true;
            while (token != NULL && in_loop) {
                char used_delim = *(copy + (token - string + strlen(token)));
                switch (used_delim) {
                    case ',':
                        in_loop = false;
                    case '|':
                        if ((*ho + *num)->ho.slider.curves != NULL) {
                            (*ho + *num)->ho.slider.curves = realloc((*ho + *num)->ho.slider.curves, ((*ho + *num)->ho.slider.num_curve + 1) * sizeof(HOSliderCurve));
                            ((*ho + *num)->ho.slider.curves + (*ho + *num)->ho.slider.num_curve - 1)->y = (int) strtol(token, NULL, 10);
                        }
                        break;
                    
                    case ':':
                        ((*ho + *num)->ho.slider.curves + (*ho + *num)->ho.slider.num_curve)->x = (int) strtol(token, NULL, 10);
                        (*ho + *num)->ho.slider.num_curve++;
                        break;
                }
                if (in_loop) {
                    token = strtok(NULL, ":|,");
                }
            }
            (*ho + *num)->ho.slider.slides = (int) strtol(strtok(NULL, ","), NULL, 10);
            // `length,edgeSounds,edgeSets` can be removed from a slider and still retain the default propertise
            if ((token = strtok(NULL, ",")) != NULL) {
                (*ho + *num)->ho.slider.length = strtod(token, NULL);
            }
            if ((token = strtok(NULL, "|,")) != NULL) {
                (*ho + *num)->ho.slider.edge_sounds = calloc(1, sizeof(int));
                (*ho + *num)->ho.slider.num_edge_sound = 0;
                in_loop = true;
                while (token != NULL && in_loop) {
                    char used_delim = *(copy + (token - string + strlen(token)));
                    switch (used_delim) {
                        case ',':
                            in_loop = false;
                        case '|':
                            if ((*ho + *num)->ho.slider.edge_sounds != NULL) {
                                (*ho + *num)->ho.slider.edge_sounds = realloc((*ho + *num)->ho.slider.edge_sounds, ((*ho + *num)->ho.slider.num_edge_sound + 1) * sizeof(int));
                                *((*ho + *num)->ho.slider.edge_sounds + (*ho + *num)->ho.slider.num_edge_sound) = (int) strtol(token, NULL, 10);
                                (*ho + *num)->ho.slider.num_edge_sound++;
                            }
                            break;
                    }
                    if (in_loop) {
                        token = strtok(NULL, "|,");
                    }
                }
            }
            if ((token = strtok(NULL, ":|,")) != NULL && *(copy + (token - string + strlen(token))) == ':') {
                (*ho + *num)->ho.slider.edge_sets = calloc(1, sizeof(HOSliderEdge));
                (*ho + *num)->ho.slider.num_edge_set = 0;
                in_loop = true;
                while (token != NULL && in_loop) {
                    char used_delim = *(copy + (token - string + strlen(token)));
                    switch (used_delim) {
                        case '\0':
                        case ',':
                            in_loop = false;
                        case '|':
                            ((*ho + *num)->ho.slider.edge_sets + (*ho + *num)->ho.slider.num_edge_set)->additional = (int) strtol(token, NULL, 10);
                            (*ho + *num)->ho.slider.num_edge_set++;
                            break;
                        
                        case ':':
                            if ((*ho + *num)->ho.slider.edge_sets != NULL) {
                                (*ho + *num)->ho.slider.edge_sets = realloc((*ho + *num)->ho.slider.edge_sets, ((*ho + *num)->ho.slider.num_edge_set + 1) * sizeof(HOSliderEdge));
                                ((*ho + *num)->ho.slider.edge_sets + (*ho + *num)->ho.slider.num_edge_set)->normal = (int) strtol(token, NULL, 10);
                            }
                            break;
                    }
                    if (in_loop) {
                        token = strtok(NULL, ":|,");
                    }
                }
            }
        } else if ((*ho + *num)->type == 8
            || (*ho + *num)->type == 12) {
            (*ho + *num)->ho_type = spinner;
            (*ho + *num)->ho.spinner.end_time = (int) strtol(strtok(NULL, ","), NULL, 10);
        }
        // If provided string has `hitSample` section (since it can be defaulted)
        if ((token = strtok(NULL, ":")) != NULL) {
            (*ho + *num)->hit_sample.normal_set = (int) strtol(token, NULL, 10);
            (*ho + *num)->hit_sample.addition_set = (int) strtol(strtok(NULL, ":"), NULL, 10);
            (*ho + *num)->hit_sample.index = (int) strtol(strtok(NULL, ":"), NULL, 10);
            (*ho + *num)->hit_sample.volume = (int) strtol(strtok(NULL, ":"), NULL, 10);
            if ((token = strtok(NULL, ":")) != NULL) {
                (*ho + *num)->hit_sample.filename = strdup(token);
            }
        }
        (*num)++;
    }
    free(copy);
}

void ofb_hitobjects_free(HitObject *ho, unsigned int num) {
    if (ho != NULL) {
        for (int i = 0; i < num; i++) {
            if ((ho + i)->ho_type == slider) {
                if ((ho + i)->ho.slider.curves != NULL) {
                    free((ho + i)->ho.slider.curves);
                    (ho + i)->ho.slider.curves = NULL;
                }
                if ((ho + i)->ho.slider.edge_sounds != NULL) {
                    free((ho + i)->ho.slider.edge_sounds);
                    (ho + i)->ho.slider.edge_sounds = NULL;
                }
                if ((ho + i)->ho.slider.edge_sets != NULL) {
                    free((ho + i)->ho.slider.edge_sets);
                    (ho + i)->ho.slider.edge_sets = NULL;
                }
            }
            if ((ho + i)->hit_sample.filename != NULL) {
                free((ho + i)->hit_sample.filename);
                (ho + i)->hit_sample.filename = NULL;
            }
        }
        free(ho);
        ho = NULL;
    }
}

void ofb_hitobjects_sort(HitObject *ho, unsigned int num) {
    for (int i = 0; i < num - 1; i++) {
        for (int j = i + 1; j < num; j++) {
            if ((ho + i)->time > (ho + j)->time) {
                HitObject temp = *(ho + i);
                *(ho + i) = *(ho + j);
                *(ho + j) = temp;
            }
        }
    }
}

void ofb_hitobjects_tofile(HitObject *ho, unsigned int num, FILE *fp) {
    fputs("[HitObjects]\n", fp);
    for (int i = 0; i < num; i++) {
        char *output;
        int size_x = ou_comparing_size((ho + i)->x);
        int size_y = ou_comparing_size((ho + i)->y);
        int size_time = ou_comparing_size((ho + i)->time);
        int size_type = ou_comparing_size((ho + i)->type);
        int size_hitsound = ou_comparing_size((ho + i)->hit_sound);
        int size_total = size_x + 1 + size_y + 1 + size_time + 1 + size_type + 1 + size_hitsound;
        bool with_sample = false;
        int size_hitsample = 0;
        if ((ho + i)->hit_sample.normal_set != 0 ||
            (ho + i)->hit_sample.addition_set != 0 ||
            (ho + i)->hit_sample.index != 0 ||
            (ho + i)->hit_sample.volume != 0 ||
            (ho + i)->hit_sample.filename != NULL) {
                with_sample = true;
                int size_normalset = ou_comparing_size((ho + i)->hit_sample.normal_set);
                int size_additionset = ou_comparing_size((ho + i)->hit_sample.addition_set);
                int size_index = ou_comparing_size((ho + i)->hit_sample.index);
                int size_volume = ou_comparing_size((ho + i)->hit_sample.volume);
                int size_filename = ((ho + i)->hit_sample.filename != NULL ? strlen((ho + i)->hit_sample.filename) : 0);
                size_hitsample = 1 + size_normalset + 1 + size_additionset + 1 + size_index + 1 + size_volume + 1 + size_filename;
        }
        int len = size_total + 1;
        output = malloc((len + 1) * sizeof(char));
        snprintf(output, len, "%d,%d,%d,%d,%d",
            (ho + i)->x,
            (ho + i)->y,
            (ho + i)->time,
            (ho + i)->type,
            (ho + i)->hit_sound
        );
        switch ((ho + i)->ho_type) {
            case circle:
                break;

            case slider: {
                int size_slider = 1;
                char *buffer = malloc((size_slider + 1) * sizeof(char));
                
                {
                    size_slider += 1 + 1;
                    snprintf(buffer, size_slider, ",%c", (ho + i)->ho.slider.curve_type);
                }
                
                for (int j = 0; j < (ho + i)->ho.slider.num_curve; j++) {
                    int size_slider_x = ou_comparing_size(((ho + i)->ho.slider.curves + j)->x);
                    int size_slider_y = ou_comparing_size(((ho + i)->ho.slider.curves + j)->y);
                    int size_slider_curve = 1 + size_slider_x + 1 + size_slider_y + 1;
                    char *buffer_curve = malloc((size_slider_curve + 1) * sizeof(char));
                    snprintf(buffer_curve, size_slider_curve, "|%d:%d", ((ho + i)->ho.slider.curves + j)->x, ((ho + i)->ho.slider.curves + j)->y);

                    size_slider += size_slider_curve;
                    buffer = realloc(buffer, (size_slider + 1) * sizeof(char));
                    strcat(buffer, buffer_curve);

                    free(buffer_curve);
                }

                {
                    int size_slider_slides = ou_comparing_size((ho + i)->ho.slider.slides);
                    int size_slider_length = ou_comparing_size((int) (ho + i)->ho.slider.length);
                    int trailing_zeros = ou_comparing_trailing((ho + i)->ho.slider.length, size_slider_length);
                    int size_slider_sl = 1 + size_slider_slides + 1 + (size_slider_length + (trailing_zeros != 0 ? 1 : 0) + trailing_zeros + 1);
                    char *buffer_sl = malloc((size_slider_sl + 1) * sizeof(char));
                    snprintf(buffer_sl, size_slider_sl, ",%d,%.*f", (ho + i)->ho.slider.slides, trailing_zeros, (ho + i)->ho.slider.length);

                    size_slider += size_slider_sl;
                    buffer = realloc(buffer, (size_slider + 1) * sizeof(char));
                    strcat(buffer, buffer_sl);

                    free(buffer_sl);
                }

                if ((ho + i)->ho.slider.edge_sounds != NULL || (ho + i)->ho.slider.num_edge_sound > 0) {
                    for (int j = 0; j < (ho + i)->ho.slider.num_edge_sound; j++) {
                        int size_slider_edge_sound = ou_comparing_size(*((ho + i)->ho.slider.edge_sounds + j));
                        int len_edge_sound = 1 + size_slider_edge_sound + 1;
                        char *buffer_edge_sound = malloc((len_edge_sound + 1) * sizeof(char));
                        snprintf(buffer_edge_sound, len_edge_sound, (j != 0 ? "|%d" : ",%d"), *((ho + i)->ho.slider.edge_sounds + j));

                        size_slider += len_edge_sound;
                        buffer = realloc(buffer, (size_slider + 1) * sizeof(char));
                        strcat(buffer, buffer_edge_sound);

                        free(buffer_edge_sound);
                    }

                    if ((ho + i)->ho.slider.edge_sounds != NULL || (ho + i)->ho.slider.num_edge_sound > 0) {
                        for (int j = 0; j < (ho + i)->ho.slider.num_edge_set; j++) {
                            int size_slider_edge_set_normal = ou_comparing_size(((ho + i)->ho.slider.edge_sets + j)->normal);
                            int size_slider_edge_set_additional = ou_comparing_size(((ho + i)->ho.slider.edge_sets + j)->additional);
                            int len_edge_set = 1 + size_slider_edge_set_normal + 1 + size_slider_edge_set_additional + 1;
                            char *buffer_edge_set = malloc((len_edge_set + 1) * sizeof(char));
                            snprintf(buffer_edge_set, len_edge_set, (j != 0 ? "|%d:%d" : ",%d:%d"), ((ho + i)->ho.slider.edge_sets + j)->normal, ((ho + i)->ho.slider.edge_sets + j)->additional);
                        
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
                int size_spinner = ou_comparing_size((ho + i)->ho.spinner.end_time);
                int len_buffer = 1 + size_spinner;
                char *buffer = malloc((len_buffer + 1) * sizeof(char));
                snprintf(buffer, len_buffer, ",%d", (ho + i)->ho.spinner.end_time);

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
                (ho + i)->hit_sample.normal_set,
                (ho + i)->hit_sample.addition_set,
                (ho + i)->hit_sample.index,
                (ho + i)->hit_sample.volume,
                (ho + i)->hit_sample.filename != NULL ? (ho + i)->hit_sample.filename : ""
            );

            len += len_buffer;
            output = realloc(output, (len + 1) * sizeof(char));
            strcat(output, buffer);

            free(buffer);
        }
        strcat(output, "\n");

        fputs(output, fp);
        free(output);
    }
}