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
            memset((*ho + *num), 0, sizeof(HitObject)); // TODO Check if this works
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
        } else if ((*ho+ *num)->type == 2
            || (*ho+ *num)->type == 6) {
            (*ho+ *num)->ho_type = slider;
            (*ho+ *num)->ho.slider.curve_type = strtok(NULL, "|")[0];
            (*ho+ *num)->ho.slider.curves = calloc(1, sizeof(HOSliderCurve));
            (*ho+ *num)->ho.slider.num_curve = 0;
            token = strtok(NULL, ":|,");
            bool in_loop = true;
            while (token != NULL && in_loop) {
                char used_delim = *(copy + (token - string + strlen(token)));
                switch (used_delim) {
                    case ',':
                        in_loop = false;
                    case '|':
                        if ((*ho+ *num)->ho.slider.curves != NULL) {
                            (*ho+ *num)->ho.slider.curves = realloc((*ho+ *num)->ho.slider.curves, ((*ho+ *num)->ho.slider.num_curve + 1) * sizeof(HOSliderCurve));
                            ((*ho+ *num)->ho.slider.curves + (*ho+ *num)->ho.slider.num_curve)->x = (int) strtol(token, NULL, 10);
                        }
                        break;
                    
                    case ':':
                        ((*ho+ *num)->ho.slider.curves + (*ho+ *num)->ho.slider.num_curve)->y = (int) strtol(token, NULL, 10);
                        (*ho+ *num)->ho.slider.num_curve++;
                        break;
                }
                if (in_loop) {
                    token = strtok(NULL, ":|,");
                }
            }
            (*ho+ *num)->ho.slider.slides = (int) strtol(strtok(NULL, ","), NULL, 10);
            // `length,edgeSounds,edgeSets` can be removed from a slider and still retain the default propertise
            if ((token = strtok(NULL, ",")) != NULL) {
                (*ho+ *num)->ho.slider.length = strtod(token, NULL);
            }
            if ((token = strtok(NULL, "|,")) != NULL) {
                (*ho+ *num)->ho.slider.edge_sounds = calloc(1, sizeof(int));
                (*ho+ *num)->ho.slider.num_edge_sound = 0;
                in_loop = true;
                while (token != NULL && in_loop) {
                    char used_delim = *(copy + (token - string + strlen(token)));
                    switch (used_delim) {
                        case ',':
                            in_loop = false;
                        case '|':
                            if ((*ho+ *num)->ho.slider.edge_sounds != NULL) {
                                (*ho+ *num)->ho.slider.edge_sounds = realloc((*ho+ *num)->ho.slider.edge_sounds, ((*ho+ *num)->ho.slider.num_edge_sound + 1) * sizeof(int));
                                *((*ho+ *num)->ho.slider.edge_sounds + (*ho+ *num)->ho.slider.num_edge_sound) = (int) strtol(token, NULL, 10);
                                (*ho+ *num)->ho.slider.num_edge_sound++;
                            }
                            break;
                    }
                    if (in_loop) {
                        token = strtok(NULL, "|,");
                    }
                }
            }
            if ((token = strtok(NULL, "|,")) != NULL) {
                (*ho+ *num)->ho.slider.edge_sets = calloc(1, sizeof(HOSliderEdge));
                (*ho+ *num)->ho.slider.num_edge_set = 0;
                in_loop = true;
                while (token != NULL && in_loop) {
                    char used_delim = *(copy + (token - string + strlen(token)));
                    switch (used_delim) {
                        case ',':
                            in_loop = false;
                        case '|':
                            if ((*ho+ *num)->ho.slider.edge_sets != NULL) {
                                (*ho+ *num)->ho.slider.edge_sets = realloc((*ho+ *num)->ho.slider.edge_sets, ((*ho+ *num)->ho.slider.num_edge_set + 1) * sizeof(HOSliderEdge));
                                ((*ho+ *num)->ho.slider.edge_sets + (*ho+ *num)->ho.slider.num_edge_set)->normal = (int) strtol(token, NULL, 10);
                            }
                            break;
                        
                        case ':':
                            ((*ho+ *num)->ho.slider.edge_sets + (*ho+ *num)->ho.slider.num_edge_set)->additional = (int) strtol(token, NULL, 10);
                            (*ho+ *num)->ho.slider.num_edge_set++;
                            break;
                    }
                    if (in_loop) {
                        token = strtok(NULL, ":|,");
                    }
                }
            }
        } else if ((*ho+ *num)->type == 8
            || (*ho+ *num)->type == 12) {
            (*ho+ *num)->ho_type = spinner;
            (*ho+ *num)->ho.spinner.end_time = (int) strtol(strtok(NULL, ","), NULL, 10);
        }
        // If provided string has `hitSample` section (since it can be defaulted)
        token = strtok(NULL, ":");
        if (token != NULL) {
            (*ho+ *num)->hit_sample.normal_set = (int) strtol(token, NULL, 10);
            (*ho+ *num)->hit_sample.addition_set = (int) strtol(strtok(NULL, ":"), NULL, 10);
            (*ho+ *num)->hit_sample.index = (int) strtol(strtok(NULL, ":"), NULL, 10);
            (*ho+ *num)->hit_sample.volume = (int) strtol(strtok(NULL, ":"), NULL, 10);
            token = strtok(NULL, ":");
            if (token != NULL) {
                (*ho+ *num)->hit_sample.filename = strdup(token);
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
        int len_x = ((ho + i)->x == 0 ? 1 : (floor(log10(abs((ho + i)->x))) + 1 + ((ho + i)->x < 0 ? 1 : 0)));
        int len_y = ((ho + i)->y == 0 ? 1 : (floor(log10(abs((ho + i)->y))) + 1 + ((ho + i)->y < 0 ? 1 : 0)));
        int len_time = ((ho + i)->time == 0 ? 1 : (floor(log10(abs((ho + i)->time))) + 1 + ((ho + i)->time < 0 ? 1 : 0)));
        int len_type = ((ho + i)->type == 0 ? 1 : (floor(log10(abs((ho + i)->type))) + 1 + ((ho + i)->type < 0 ? 1 : 0)));
        int len_hitsound = ((ho + i)->hit_sound == 0 ? 1 : (floor(log10(abs((ho + i)->hit_sound))) + 1 + ((ho + i)->hit_sound < 0 ? 1 : 0)));
        int len_total = len_x + 1 + len_y + 1 + len_time + 1 + len_type + 1 + len_hitsound;
        bool with_sample = false;
        int len_hitsample = 0;
        if ((ho + i)->hit_sample.normal_set != 0 ||
            (ho + i)->hit_sample.addition_set != 0 ||
            (ho + i)->hit_sample.index != 0 ||
            (ho + i)->hit_sample.volume != 0 ||
            (ho + i)->hit_sample.filename != NULL) {
                with_sample = true;
                int len_normalset = ((ho + i)->hit_sample.normal_set == 0 ? 1 : (floor(log10(abs((ho + i)->hit_sample.normal_set))) + 1 + ((ho + i)->hit_sample.normal_set < 0 ? 1 : 0)));
                int len_additionset = ((ho + i)->hit_sample.addition_set == 0 ? 1 : (floor(log10(abs((ho + i)->hit_sample.addition_set))) + 1 + ((ho + i)->hit_sample.addition_set < 0 ? 1 : 0)));
                int len_index = ((ho + i)->hit_sample.index == 0 ? 1 : (floor(log10(abs((ho + i)->hit_sample.index))) + 1 + ((ho + i)->hit_sample.index < 0 ? 1 : 0)));
                int len_volume = ((ho + i)->hit_sample.volume == 0 ? 1 : (floor(log10(abs((ho + i)->hit_sample.volume))) + 1 + ((ho + i)->hit_sample.volume < 0 ? 1 : 0)));
                int len_filename = ((ho + i)->hit_sample.filename != NULL ? strlen((ho + i)->hit_sample.filename) : 0);
                len_hitsample = 1 + len_normalset + 1 + len_additionset + 1 + len_index + 1 + len_volume + 1 + len_filename;
        }
        switch ((ho + i)->ho_type) {
            case circle:
                if (with_sample) {
                    int len = len_total + len_hitsample + 1;
                    output = malloc((len + 1) * sizeof(char));
                    snprintf(output, len, "%d,%d,%d,%d,%d,%d:%d:%d:%d:%s",
                        (ho + i)->x,
                        (ho + i)->y,
                        (ho + i)->time,
                        (ho + i)->type,
                        (ho + i)->hit_sound,
                        (ho + i)->hit_sample.normal_set,
                        (ho + i)->hit_sample.addition_set,
                        (ho + i)->hit_sample.index,
                        (ho + i)->hit_sample.volume,
                        (ho + i)->hit_sample.filename
                    );
                    strcat(output, "\n");
                } else {
                    int len = len_total + 1;
                    output = malloc((len + 1) * sizeof(char));
                    snprintf(output, len, "%d,%d,%d,%d,%d",
                        (ho + i)->x,
                        (ho + i)->y,
                        (ho + i)->time,
                        (ho + i)->type,
                        (ho + i)->hit_sound
                    );
                    strcat(output, "\n");
                }
                break;

            // TODO
            case slider:
                if (with_sample) {
                    output = malloc((len_total + 2) * sizeof(char));
                } else {
                    output = malloc((len_total + 2) * sizeof(char));
                }
                break;

            case spinner:
                if (with_sample) {
                    int len = len_total + 1 + len_hitsample + 1;
                    output = malloc((len + 1) * sizeof(char));
                    snprintf(output, len, "%d,%d,%d,%d,%d,%d,%d:%d:%d:%d:%s",
                        (ho + i)->x,
                        (ho + i)->y,
                        (ho + i)->time,
                        (ho + i)->type,
                        (ho + i)->hit_sound,
                        (ho + i)->ho.spinner.end_time,
                        (ho + i)->hit_sample.normal_set,
                        (ho + i)->hit_sample.addition_set,
                        (ho + i)->hit_sample.index,
                        (ho + i)->hit_sample.volume,
                        (ho + i)->hit_sample.filename
                    );
                    strcat(output, "\n");
                } else {
                    int len = len_total + 1 + 1;
                    output = malloc((len + 1) * sizeof(char));
                    snprintf(output, len, "%d,%d,%d,%d,%d,%d",
                        (ho + i)->x,
                        (ho + i)->y,
                        (ho + i)->time,
                        (ho + i)->type,
                        (ho + i)->hit_sound,
                        (ho + i)->ho.spinner.end_time
                    );
                    strcat(output, "\n");
                }
                break;
        }
        fputs(output, fp);
        free(output);
    }
    fputs("\n", fp);
}