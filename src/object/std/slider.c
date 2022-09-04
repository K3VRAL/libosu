// https://www.geeksforgeeks.org/coroutines-in-c-cpp/
#include "object/std.h"

void oos_slider_init(Slider *slider, Difficulty difficulty, InheritedTimingPoint *inherited, UninheritedTimingPoint *uninherited, HitObject *hit_object) {
    TimingPoint *tp_inherited = NULL;
    oos_timingpoint_attime(tp_inherited, hit_object->time, inherited->tp, inherited->num);
    
    TimingPoint *tp_uninherited = NULL;
    oos_timingpoint_attime(tp_uninherited, hit_object->time, uninherited->tp, uninherited->num);
    
    double scoring_distance = oos_hitobject_BASESCORINGDISTANCE * difficulty.slider_multiplier * (-100 / tp_inherited->beat_length);
    slider = malloc(sizeof(*slider));
    slider->tick_distance_multiplier = 1;
    slider->velocity = scoring_distance / tp_uninherited->beat_length;
    slider->tick_distance = scoring_distance / difficulty.slider_tick_rate * slider->tick_distance_multiplier;
    slider->path.distance = hit_object->ho.slider.length;
    slider->start_time = hit_object->time;
    slider->span_count = hit_object->ho.slider.slides;
    slider->end_time = slider->start_time + slider->span_count * slider->path.distance / slider->velocity;
    slider->duration = slider->end_time - slider->start_time;
    slider->span_duration = slider->duration / slider->span_count;
    slider->legacy_last_tick_offset = malloc(sizeof(*slider->legacy_last_tick_offset));
    *slider->legacy_last_tick_offset = 36;
    slider->start_position.x = hit_object->x;
    slider->start_position.y = hit_object->y;

    slider->ho_data = malloc(sizeof(*slider->ho_data));
    slider->ho_data->curve_type = hit_object->ho.slider.curve_type;
    slider->ho_data->curves = hit_object->ho.slider.curves;
    slider->ho_data->num_curve = hit_object->ho.slider.num_curve;

    slider->nested = NULL;
    slider->num_nested = 0;

    oos_timingpoint_free(tp_inherited);
    oos_timingpoint_free(tp_uninherited);
}

void oos_slider_initwouninandinherited(Slider *slider, Difficulty difficulty, TimingPoint *timing_point, unsigned int num, HitObject *hit_object) {
    InheritedTimingPoint *inherited = NULL;
    oos_inheritedpoint_init(inherited, timing_point, num);

    UninheritedTimingPoint *uninherited = NULL;
    oos_uninheritedpoint_init(uninherited, timing_point, num);
    
    oos_slider_init(slider, difficulty, inherited, uninherited, hit_object);
    
    oos_inheritedpoint_free(inherited);
    oos_uninheritedpoint_free(uninherited);
}

// TODO do it in this format for every other freeable object
void oos_slider_free(Slider *slider) {
    if (slider == NULL) {
        return;
    }
    if (slider->legacy_last_tick_offset != NULL) {
        free(slider->legacy_last_tick_offset);
    }
    // TODO figure out a way to free all of `nested`
    free(slider);
}


void oos_slider_catmullfindpoint(SliderVector2 *result, SliderVector2 *v1, SliderVector2 *v2, SliderVector2 *v3, SliderVector2 *v4, float t) {
    float t2 = t * t;
    float t3 = t * t2;
    result = malloc(sizeof(*result));
    result->x = 0.5 * (2 * v2->x + (-v1->x + v3->x) * t + (2 * v1->x - 5 * v2->x + 4 * v3->x - v4->x) * t2 + (-v1->x + 3 * v2->x - 3 * v3->x + v4->x) * t3);
    result->y = 0.5 * (2 * v2->y + (-v1->y + v3->y) * t + (2 * v1->y - 5 * v2->y + 4 * v3->y - v4->y) * t2 + (-v1->y + 3 * v2->y - 3 * v3->y + v4->y) * t3);
}

void oos_slider_catmullapproximate(SliderVector2 *result, unsigned int *len_result, SliderVector2 *vertices, unsigned int *len) {
    int catmull_detail = 50;
    int catmull_len = (*len - 1) * catmull_detail * 2;
    result = malloc(catmull_len * sizeof(*result));
    for (int i = 0; i < *len - 1; i++) {
        SliderVector2 v1;
        if (i > 0) {
            v1 = vertices[i - 1];
        } else {
            v1 = vertices[i];
        }
        
        SliderVector2 v2 = vertices[i];
        SliderVector2 v3;
        if (i < *len - 1) {
            v3 = vertices[i + 1];
        } else {
            v3.x = v2.x + v2.x - v1.x;
            v3.y = v2.y + v2.y - v1.y;
        }
        SliderVector2 v4;
        if (i < *len - 2) {
            v4 = vertices[i + 2];
        } else {
            v4.x = v3.x + v3.x - v2.x;
            v4.y = v3.y + v3.y - v2.y;
        }

        for (int c = 0; c < catmull_detail; c++) {
            SliderVector2 *catmull_result = NULL;
            oos_slider_catmullfindpoint(catmull_result, &v1, &v2, &v3, &v4, (float) c / catmull_detail);
            (result + *len_result)->x = catmull_result->x;
            (result + *len_result)->y = catmull_result->y;
            (*len_result)++;
            free(catmull_result);

            oos_slider_catmullfindpoint(catmull_result, &v1, &v2, &v3, &v4, (float) (c + 1) / catmull_detail);
            (result + *len_result)->x = catmull_result->x;
            (result + *len_result)->y = catmull_result->y;
            (*len_result)++;
            free(catmull_result);
        }
    }
}

// TODO
void oos_slider_calculatesubpath(SliderVector2 *sub_path, unsigned int *len_subpath, SliderVector2 *vertices, unsigned int *len, SliderType type) {
    switch (type) {
        case slidertype_linear:
            sub_path->type = vertices->type;
            sub_path->x = vertices->x;
            sub_path->y = vertices->y;
            *len_subpath = *len;
            return;

        case slidertype_bezier:
            break;

        case slidertype_perfectcurve:
        //     if (*len != 3) {
        //         break;
        //     }
        //     oos_slider_approximatecirculararc(sub_path, len_subpath, vertices, len);
        //     if (*len_subpath == 0) {
        //         break;
        //     }
            return;
        
        case slidertype_catmull:
            oos_slider_catmullapproximate(sub_path, len_subpath, vertices, len);
            return;
    }
    // oos_slider_approximatebezier(sub_path, len_subpath, vertices, len);
}

void oos_slider_positionat(SliderVector2 *vector, double progress, Slider *slider) {
    SliderVector2 *calculate_path = NULL;
    unsigned int calculatepath_len = 0;

    // calculatePath()
    int controlpoint_len = slider->ho_data->num_curve + 1;
    SliderVector2 control_point[controlpoint_len];
    control_point[0].type = &slider->ho_data->curve_type;
    control_point[0].x = 0;
    control_point[0].y = 0;
    for (int i = 1; i < controlpoint_len; i++) {
        control_point[i].x = (slider->ho_data->curves + i - 1)->x;
        control_point[i].y = (slider->ho_data->curves + i - 1)->y;
    }
    int start = 0;
    for (int i = 0; i < controlpoint_len; i++) {
        if (control_point[i].type == NULL && i < controlpoint_len - 1) {
            continue;
        }
        unsigned int segment_len = i - start + 1;
        SliderVector2 segment_vertices[segment_len];
        for (int i = start; i < segment_len; i++) {
            segment_vertices[i].x = control_point[i].x;
            segment_vertices[i].y = control_point[i].y;
        }
        SliderType segment_type = control_point[start].type != NULL ? *control_point[start].type : slidertype_linear;
        
        SliderVector2 *result = NULL;
        unsigned int len_result = 0;
        oos_slider_calculatesubpath(result, &len_result, segment_vertices, &segment_len, segment_type);
        for (int i = 0; i < len_result; i++) {
            if (calculatepath_len == 0
                || ((calculate_path - calculatepath_len - 1)->x != (result + i)->x
                    && (calculate_path - calculatepath_len - 1)->y != (result + i)->y)) {
                calculate_path = realloc(calculate_path, (calculatepath_len + 1) * sizeof(*calculate_path));
                (calculate_path + calculatepath_len)->x = (result + i)->x;
                (calculate_path + calculatepath_len)->y = (result + i)->y;
                calculatepath_len++;
            }
        }
        free(result);
        start = i;
    }
    
    // caluclateLength()
    double calculated_length = 0;
    double *cumulative_length = malloc(sizeof(*cumulative_length));
    unsigned int cumulativelength_len = 0;
    *(cumulative_length + cumulativelength_len) = 0;
    cumulativelength_len++;
    for (int i = 0; i < calculatepath_len - 1; i++) {
        SliderVector2 diff = {
            .x = (calculate_path + i + 1)->x - (calculate_path + i)->x,
            .y = (calculate_path + i + 1)->y - (calculate_path + i)->y
        };
        calculated_length += sqrt((diff.x * diff.x) + (diff.y * diff.y));
        cumulative_length = realloc(cumulative_length, (cumulativelength_len + 1) * sizeof(*cumulative_length));
        *(cumulative_length + cumulativelength_len) = calculated_length;
        cumulativelength_len++;
    }
    if (calculated_length != slider->path.distance) { // Should I be worried with `ExpectedDistance.Value is double`? It seems that it is nullable
        if (controlpoint_len >= 2
            && ((control_point[controlpoint_len - 1].x == control_point[controlpoint_len - 2].x
                && control_point[controlpoint_len - 1].y == control_point[controlpoint_len - 2].y))
            && slider->path.distance > calculated_length) {
            cumulative_length = realloc(cumulative_length, (cumulativelength_len + 1) * sizeof(*cumulative_length));
            *(cumulative_length + cumulativelength_len) = calculated_length;
            cumulativelength_len++;
        }
        // TODO remove last item of cumulative_length
        int path_end_index = calculatepath_len - 1;
        if (calculated_length > slider->path.distance) {
            while (cumulativelength_len > 0 && *(cumulative_length + cumulativelength_len - 1) >= slider->path.distance) {
                // TODO remove last item of `cumulative_length`
                // TODO remove `path_end_index` of `calculated_path`
            }
        }
        if (path_end_index <= 0) {
            cumulative_length = realloc(cumulative_length, (cumulativelength_len + 1) * sizeof(*cumulative_length));
            *(cumulative_length + cumulativelength_len) = 0;
            cumulativelength_len++;
        }
        SliderVector2 dir = {
            .x = (calculate_path + path_end_index)->x - (calculate_path + path_end_index - 1)->x,
            .y = (calculate_path + path_end_index)->y - (calculate_path + path_end_index - 1)->y
        };
        float distance = sqrtf((dir.x * dir.x) + (dir.y * dir.y));
        dir.x = dir.x / distance;
        dir.y = dir.y / distance;
        (calculate_path + path_end_index)->x = (calculate_path + path_end_index - 1)->x + dir.x * (float) (slider->path.distance - *(cumulative_length + cumulativelength_len - 1));
        (calculate_path + path_end_index)->y = (calculate_path + path_end_index - 1)->y + dir.y * (float) (slider->path.distance - *(cumulative_length + cumulativelength_len - 1));
        cumulative_length = realloc(cumulative_length, (cumulativelength_len + 1) * sizeof(*cumulative_length));
        *(cumulative_length + cumulativelength_len) = slider->path.distance;
        cumulativelength_len++;
    }

    double d = fmax(0, fmin(progress, 1)) * slider->path.distance;
    
    // indexOfDisatnce(d)
    int i = 0;
    int l = 0;
    int r = cumulativelength_len - 2;
    while (l <= r) {
        int pivot = (r + l) / 2;
        if (*(cumulative_length + pivot) < d) {
            l = pivot + 1;
        } else if (*(cumulative_length + pivot) > d) {
            r = pivot - 1;
        } else {
            i = *(cumulative_length + pivot);
            break;
        }
    }
    if (i < 0) {
        i = ~i;
    }

    // interpolateVertices(indexOfDisatnce(d), d)
    if (calculatepath_len == 0) {
        vector->x = 0;
        vector->y = 0;
        return;
    }
    if (i <= 0) {
        vector->x = (calculate_path + 0)->x;
        vector->y = (calculate_path + 0)->y;
        return;
    }
    if (i >= calculatepath_len) {
        vector->x = (calculate_path + calculatepath_len - 1)->x;
        vector->y = (calculate_path + calculatepath_len - 1)->y;
        return;
    }
    SliderVector2 p0 = {
        .x = (calculate_path + i - 1)->x,
        .y = (calculate_path + i - 1)->y
    };
    SliderVector2 p1 = {
        .x = (calculate_path + i)->x,
        .y = (calculate_path + i)->y
    };
    double d0 = *(cumulative_length + i - 1);
    double d1 = *(cumulative_length + i);
    if (fabs(d0 - d1) <= 1e-7) {
        vector->x = p0.x;
        vector->y = p0.y;
        return;
    }
    double w = (d - d0) / (d1 - d0);
    vector->x = p0.x + (p1.x - p0.x) * (float) w;
    vector->y = p0.y + (p1.y - p0.y) * (float) w;

    // TODO was too focused on getting this done that I forgot what needs to be freed; just use valgrind for this shit
}

void oos_slider_createnestedhitobjects(Slider *slider) {
    SliderEventDescriptor *slider_event;
    while ((slider_event = oos_slider_generate(slider->start_time, slider->span_duration, slider->velocity, slider->tick_distance, slider->path.distance, slider->span_count, slider->legacy_last_tick_offset)) != NULL) {
        switch (slider_event->type) {
            case sliderevent_tick: {
                slider->nested = realloc(slider->nested, (slider->num_nested + 1) * sizeof(*slider->nested));
                // (slider->nested + slider->num_nested)->type = tick;
                (slider->nested + slider->num_nested)->time = slider_event->time;
                SliderVector2 position;
                oos_slider_positionat(&position, slider_event->path_progress, slider);
                (slider->nested + slider->num_nested)->x = position.x;
                (slider->nested + slider->num_nested)->y = position.y;
                slider->num_nested++;
                break;
            }

            case sliderevent_head: {
                // (slider->nested + slider->num_nested)->type = head;
                (slider->nested + slider->num_nested)->time = slider_event->time;
                (slider->nested + slider->num_nested)->x = slider->start_position.x;
                (slider->nested + slider->num_nested)->y = slider->start_position.y;
                slider->num_nested++;
                break;
            }

            case sliderevent_legacylasttick: {
                // (slider->nested + slider->num_nested)->type = tail;
                (slider->nested + slider->num_nested)->time = slider_event->time;
                // (slider->nested + slider->num_nested)->x = ; // TODO get `EndPosition`
                // (slider->nested + slider->num_nested)->y = ;
                slider->num_nested++;
                break;
            }

            case sliderevent_repeat: {
                // (slider->nested + slider->num_nested)->type = repeat;
                (slider->nested + slider->num_nested)->time = slider_event->time; // TODO figure out `StartTime + (e.SpanIndex + 1) * SpanDuration`
                SliderVector2 position;
                oos_slider_positionat(&position, slider_event->path_progress, slider);
                (slider->nested + slider->num_nested)->x = position.x;
                (slider->nested + slider->num_nested)->y = position.x;
                slider->num_nested++;
                break;
            }

            case sliderevent_tail:
                // Source code just ignores this
                break;
        }
    }
}

SliderEventDescriptor *oos_slider_generate(double start_time, double span_duration, double velocity, double tick_distance, double total_distance, int span_count, double *legacy_last_tick_offset) {
    static int state = 0;
    static SliderEventDescriptor *object = NULL;
    if (object != NULL) {
        free(object);
        object = NULL;
    }
    switch (state) {
        case 0:;
            const double max_length = 100000;
            static double length;
            length = fmin(max_length, total_distance);
            static double ftick_distance;
            ftick_distance = fmin(tick_distance, fmax(0, length));
            static double min_distance_from_end;
            min_distance_from_end = velocity * 10;

            {
                state = 1;
                object = malloc(sizeof(*object));
                object->type = sliderevent_head;
                object->span_index = 0;
                object->span_start_time = start_time;
                object->time = start_time;
                object->path_progress = 0;
                return object;
            }
        
        case 1:
            if (ftick_distance != 0) {
                static int span;
                for (span = 0; span < span_count; span++) {
                    static double span_start_time;
                    span_start_time = start_time + span * span_duration;
                    static bool reversed;
                    reversed = span % 2 == 1;
                    
                    {
                        state = 2;
                        while ((object = oos_slider_generateticks(span, span_start_time, span_duration, reversed, length, ftick_distance, min_distance_from_end)) != NULL) {
                            return object;
        case 2:;
                        }
                    }

                    state = 3;
                    if (span < span_count - 1) {
                        object = malloc(sizeof(*object));
                        object->type = sliderevent_repeat;
                        object->span_index = span;
                        object->span_start_time = start_time + span * span_duration;
                        object->time = span_start_time + span_duration;
                        object->path_progress = (span + 1) % 2;
                        return object;
                    }
        case 3:;
                }
            }

            static double total_duration;
            total_duration = span_count * span_duration;
            static int final_span_index;
            final_span_index = span_count - 1;
            double final_span_start_time = start_time + final_span_index * span_duration;
            double final_span_end_time = fmax(start_time + total_duration / 2, (final_span_start_time + span_duration) - (legacy_last_tick_offset != NULL ? *legacy_last_tick_offset : 0));
            double final_progress = (final_span_end_time - final_span_start_time) / span_duration;
            if (span_count % 2 == 0) {
                final_progress = 1 - final_progress;
            }
            {
                state = 4;
                object = malloc(sizeof(*object));
                object->type = sliderevent_legacylasttick;
                object->span_index = final_span_index;
                object->span_start_time = final_span_start_time;
                object->time = final_span_end_time;
                object->path_progress = final_progress;
                return object;
            }
            
        case 4:
            state = 5;
            object = malloc(sizeof(*object));
            object->type = sliderevent_tail;
            object->span_index = final_span_index;
            object->span_start_time = start_time + (span_count - 1) * span_duration;
            object->time = start_time + total_duration;
            object->path_progress = (int) span_duration % 2;
            return object;

        case 5:;
    }
    
    state = 0;
    return NULL;
}

SliderEventDescriptor *oos_slider_generateticks(int span_index, double span_start_time, double span_duration, bool reversed, double length, double tick_distance, double min_distance_from_end) {
    static int state = 0;
    static SliderEventDescriptor *object = NULL;
    switch (state) {
        case 0:;
            static double d;
            if (!reversed) {
                state = 1;
                for (d = tick_distance; d <= length; d += tick_distance) {
                    if (d >= length - min_distance_from_end) {
                        break;
                    }
                    double path_progress = d / length;
                    double time_progress = reversed ? 1 - path_progress : path_progress;

                    object = malloc(sizeof(*object));
                    object->type = sliderevent_tick;
                    object->span_index = span_index;
                    object->span_start_time = span_start_time;
                    object->time = span_start_time + time_progress * span_duration;
                    object->path_progress = path_progress;
                    return object;
        case 1:;
                }
            } else {
                state = 2;
                for (d = length; d >= tick_distance; d -= tick_distance) {
                    if (d >= length - min_distance_from_end) {
                        break;
                    }
                    double path_progress = d / length;
                    double time_progress = reversed ? 1 - path_progress : path_progress;

                    object = malloc(sizeof(*object));
                    object->type = sliderevent_tick;
                    object->span_index = span_index;
                    object->span_start_time = span_start_time;
                    object->time = span_start_time + time_progress * span_duration;
                    object->path_progress = path_progress;
                    return object;
        case 2:;
                }
            }
    }
    state = 0;
    return NULL;
}