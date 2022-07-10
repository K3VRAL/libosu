#include "file/hit_objects.h"

HitObjects osuh_init() {
    HitObjects hit_objects = {
        .hit_objects = NULL,
        .num = 0
    };
    return hit_objects;
}

// TODO
void osuh_free(HitObjects *ho) {
}

// TODO
void osuh_set(HitObjects *ho, char *syntax) {
    osuh_add_HO(ho, syntax);
}

void osuh_add_HO(HitObjects *ho, char *string) {
    char *copy = strdup(string);
    char *token = strtok(string, ",");
    if (ho->hit_objects == NULL || ho->num == 0) {
        // If there is no space; make space
        ho->hit_objects = malloc(sizeof(HitObjects));
    } else {
        // If there is space; make more space
        ho->hit_objects = realloc(ho->hit_objects, (ho->num + 1) * sizeof(HitObjects));
    }
    // Set data
    (ho->hit_objects + ho->num)->x = (int) strtol(token, NULL, 10);
    (ho->hit_objects + ho->num)->y = (int) strtol(strtok(NULL, ","), NULL, 10);
    (ho->hit_objects + ho->num)->time = (int) strtol(strtok(NULL, ","), NULL, 10);
    (ho->hit_objects + ho->num)->type = (int) strtol(strtok(NULL, ","), NULL, 10);
    (ho->hit_objects + ho->num)->hit_sound = (int) strtol(strtok(NULL, ","), NULL, 10);
    if ((ho->hit_objects + ho->num)->type == 1
        || (ho->hit_objects + ho->num)->type == 5) {
        (ho->hit_objects + ho->num)->ho_type = circle;
    } else if ((ho->hit_objects + ho->num)->type == 2
        || (ho->hit_objects + ho->num)->type == 6) {
        (ho->hit_objects + ho->num)->ho_type = slider;
        (ho->hit_objects + ho->num)->ho.slider.curve_type = strtok(NULL, "|")[0];
        (ho->hit_objects + ho->num)->ho.slider.curves = malloc(sizeof(HOSliderCurve));
        token = strtok(NULL, ":|,");
        bool in_loop = true;
        while (token != NULL && in_loop) {
            char used_delim = *(copy + (token - string + strlen(token)));
            switch (used_delim) {
                case ',':
                    in_loop = false;
                case '|':
                    ((ho->hit_objects + ho->num)->ho.slider.curves + (ho->hit_objects + ho->num)->ho.slider.num_curve)->y = (int) strtol(token, NULL, 10);
                    (ho->hit_objects + ho->num)->ho.slider.num_curve++;
                    break;
                
                case ':':
                    (ho->hit_objects + ho->num)->ho.slider.curves = realloc((ho->hit_objects + ho->num)->ho.slider.curves, ((ho->hit_objects + ho->num)->ho.slider.num_curve + 1) * sizeof(HOSliderCurve));
                    ((ho->hit_objects + ho->num)->ho.slider.curves + (ho->hit_objects + ho->num)->ho.slider.num_curve)->x = (int) strtol(token, NULL, 10);
                    break;
            }
            if (!in_loop) {
                token = strtok(NULL, ":|,");
            }
        }
        (ho->hit_objects + ho->num)->ho.slider.slides = (int) strtol(strtok(NULL, ","), NULL, 10);
        // `length,edgeSounds,edgeSets` can be removed from a slider and still retain the default propertise
        if ((token = strtok(NULL, ",")) != NULL) {
            (ho->hit_objects + ho->num)->ho.slider.length = strtod(token, NULL);
        }
        if ((token = strtok(NULL, "|,")) != NULL) {
            (ho->hit_objects + ho->num)->ho.slider.edge_sounds = malloc(sizeof(int));
            in_loop = true;
            while (token != NULL && in_loop) {
                char used_delim = *(copy + (token - string + strlen(token)));
                switch (used_delim) {
                    case ',':
                        in_loop = false;
                    case '|':
                        (ho->hit_objects + ho->num)->ho.slider.edge_sounds = realloc((ho->hit_objects + ho->num)->ho.slider.edge_sounds, ((ho->hit_objects + ho->num)->ho.slider.num_edge_sound + 1) * sizeof(int));
                        *((ho->hit_objects + ho->num)->ho.slider.edge_sounds + (ho->hit_objects + ho->num)->ho.slider.num_edge_sound) = (int) strtol(token, NULL, 10);
                        (ho->hit_objects + ho->num)->ho.slider.num_edge_sound++;
                        break;
                }
                if (!in_loop) {
                    token = strtok(NULL, "|,");
                }
            }
        }
        if ((token = strtok(NULL, "|,")) != NULL) {
            (ho->hit_objects + ho->num)->ho.slider.edge_sets = malloc(sizeof(HOSliderEdge));
            in_loop = true;
            while (token != NULL && in_loop) {
                char used_delim = *(copy + (token - string + strlen(token)));
                switch (used_delim) {
                    case ',':
                        in_loop = false;
                    case '|':
                        ((ho->hit_objects + ho->num)->ho.slider.edge_sets + (ho->hit_objects + ho->num)->ho.slider.num_curve)->additional = (int) strtol(token, NULL, 10);
                        (ho->hit_objects + ho->num)->ho.slider.num_edge_set++;
                        break;
                    
                    case ':':
                        (ho->hit_objects + ho->num)->ho.slider.edge_sets = realloc((ho->hit_objects + ho->num)->ho.slider.edge_sets, ((ho->hit_objects + ho->num)->ho.slider.num_edge_set + 1) * sizeof(HOSliderEdge));
                        ((ho->hit_objects + ho->num)->ho.slider.edge_sets + (ho->hit_objects + ho->num)->ho.slider.num_edge_set)->normal = (int) strtol(token, NULL, 10);
                        break;
                }
                if (!in_loop) {
                    token = strtok(NULL, ":|,");
                }
            }
        }
    } else if ((ho->hit_objects + ho->num)->type == 8
        || (ho->hit_objects + ho->num)->type == 12) {
        (ho->hit_objects + ho->num)->ho_type = spinner;
        (ho->hit_objects + ho->num)->ho.spinner.end_time = (int) strtol(strtok(NULL, ","), NULL, 10);
    }
    // If provided string has `hitSample` section
    token = strtok(NULL, ",");
    if (token != NULL) {
        (ho->hit_objects + ho->num)->hit_sample.normal_set = (int) strtol(token, NULL, 10);
        (ho->hit_objects + ho->num)->hit_sample.addition_set = (int) strtol(strtok(NULL, ":"), NULL, 10);
        (ho->hit_objects + ho->num)->hit_sample.index = (int) strtol(strtok(NULL, ":"), NULL, 10);
        (ho->hit_objects + ho->num)->hit_sample.volume = (int) strtol(strtok(NULL, ":"), NULL, 10);
        token = strtok(NULL, ":");
        if (token != NULL) {
            (ho->hit_objects + ho->num)->hit_sample.filename = strdup(token);
        }
    }
    ho->num++;
    free(copy);
}