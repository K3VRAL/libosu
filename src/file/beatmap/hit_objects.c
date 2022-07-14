#include "file/beatmap/hit_objects.h"

void ofb_hitobjects_add(HitObject **ho, unsigned int *num, char *string) {
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
                            ((*ho + *num)->ho.slider.curves + (*ho + *num)->ho.slider.num_curve)->x = (int) strtol(token, NULL, 10);
                        }
                        break;
                    
                    case ':':
                        ((*ho + *num)->ho.slider.curves + (*ho + *num)->ho.slider.num_curve)->y = (int) strtol(token, NULL, 10);
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
            if ((token = strtok(NULL, "|,")) != NULL) {
                (*ho + *num)->ho.slider.edge_sets = calloc(1, sizeof(HOSliderEdge));
                (*ho + *num)->ho.slider.num_edge_set = 0;
                in_loop = true;
                while (token != NULL && in_loop) {
                    char used_delim = *(copy + (token - string + strlen(token)));
                    switch (used_delim) {
                        case ',':
                            in_loop = false;
                        case '|':
                            if ((*ho + *num)->ho.slider.edge_sets != NULL) {
                                (*ho + *num)->ho.slider.edge_sets = realloc((*ho + *num)->ho.slider.edge_sets, ((*ho + *num)->ho.slider.num_edge_set + 1) * sizeof(HOSliderEdge));
                                ((*ho + *num)->ho.slider.edge_sets + (*ho + *num)->ho.slider.num_edge_set)->normal = (int) strtol(token, NULL, 10);
                            }
                            break;
                        
                        case ':':
                            ((*ho + *num)->ho.slider.edge_sets + (*ho + *num)->ho.slider.num_edge_set)->additional = (int) strtol(token, NULL, 10);
                            (*ho + *num)->ho.slider.num_edge_set++;
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
        token = strtok(NULL, ":");
        if (token != NULL) {
            (*ho + *num)->hit_sample.normal_set = (int) strtol(token, NULL, 10);
            (*ho + *num)->hit_sample.addition_set = (int) strtol(strtok(NULL, ":"), NULL, 10);
            (*ho + *num)->hit_sample.index = (int) strtol(strtok(NULL, ":"), NULL, 10);
            (*ho + *num)->hit_sample.volume = (int) strtol(strtok(NULL, ":"), NULL, 10);
            token = strtok(NULL, ":");
            if (token != NULL) {
                (*ho + *num)->hit_sample.filename = strdup(token);
            }
        }
        (*num)++;
    }
    free(copy);
}

void ofb_hitobjects_free(HitObject **ho, unsigned int num) {
    if (ho != NULL) {
        // TODO fix errors from valgrind
        for (int i = 0; i < num; i++) {
            if ((*ho + i)->ho_type == slider) {
                if ((*ho + i)->ho.slider.curves != NULL) {
                    free((*ho + i)->ho.slider.curves);
                    (*ho + i)->ho.slider.curves = NULL;
                }
                if ((*ho + i)->ho.slider.edge_sounds != NULL) {
                    free((*ho + i)->ho.slider.edge_sounds);
                    (*ho + i)->ho.slider.edge_sounds = NULL;
                }
                if ((*ho + i)->ho.slider.edge_sets != NULL) {
                    free((*ho + i)->ho.slider.edge_sets);
                    (*ho + i)->ho.slider.edge_sets = NULL;
                }
            }
            if ((*ho + i)->hit_sample.filename != NULL) {
                free((*ho + i)->hit_sample.filename);
                (*ho + i)->hit_sample.filename = NULL;
            }
        }
        free(*ho);
        *ho = NULL;
    }
}

void ofb_hitobjects_sort(HitObject **ho, unsigned int num) {
    for (int i = 0; i < num - 1; i++) {
        for (int j = i + 1; j < num; j++) {
            if ((*ho + i)->time > (*ho + j)->time) {
                HitObject temp = *(*ho + i);
                *(*ho + i) = *(*ho + j);
                *(*ho + j) = temp;
            }
        }
    }
}