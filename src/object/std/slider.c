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
    slider->based_on = hit_object;
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

// void oos_slider_approximatecirculararc(SliderVector2 *result, unsigned int *len_result, SliderVector2 *vertices, unsigned int *len) {
//     int catmull_detail = 50;
//     int catmull_len = (*len - 1) * catmull_detail * 2;
//     result = malloc(catmull_len * sizeof(*result));
//     for (int i = 0; i < *len - 1; i++) {
//         SliderVector2 v1;
//         if (i > 0) {
//             v1 = vertices[i - 1];
//         } else {
//             v1 = vertices[i];
//         }
        
//         SliderVector2 v2 = vertices[i];
//         SliderVector2 v3;
//         if (i < *len - 1) {
//             v3 = vertices[i + 1];
//         } else {
//             v3.x = v2.x + v2.x - v1.x;
//             v3.y = v2.y + v2.y - v1.y;
//         }
//         SliderVector2 v4;
//         if (i < *len - 2) {
//             v4 = vertices[i + 2];
//         } else {
//             v4.x = v3.x + v3.x - v2.x;
//             v4.y = v3.y + v3.y - v2.y;
//         }

//         for (int c = 0; c < catmull_detail; c++) {
//             *(result + )
//         }
//     }
// }

// void oos_slider_calculatesubpath(SliderVector2 *sub_path, unsigned int *len_subpath, SliderVector2 *vertices, unsigned int *len, SliderType type) {
//     switch (type) {
//         case slidertype_linear:
//             sub_path = vertices;
//             *len_subpath = *len;
//             return;

//         case slidertype_perfectcurve:
//             if (*len != 3) {
//                 break;
//             }
//             sub_path = oos_slider_approximatecirculararc(len_subpath, vertices, len);
//             if (*len_subpath == 0) {
//                 break;
//             }
//             return ;
        
//         case slidertype_catmull:
//             return oos_slider_approximatecatmull(len_subpath, vertices, len);
//     }
//     return oos_slider_approximatebezier(vertices, len);
// }

// TODO + rewrite all of these into their own functions
// int oos_slider_positionat(double progress, Slider *slider) {
//     // calculatePath()
//     int len_cp = slider->based_on->ho.slider.num_curve + 1;
//     SliderVector2 control_point[len_cp];
//     control_point[0].type = &slider->based_on->ho.slider.curve_type;
//     control_point[0].x = 0;
//     control_point[0].y = 0;
//     for (int i = 1; i < len_cp; i++) {
//         control_point[i].x = (slider->based_on->ho.slider.curves + i - 1)->x;
//         control_point[i].y = (slider->based_on->ho.slider.curves + i - 1)->y;
//     }
//     int start = 0;
//     for (int i = 0; i < len_cp; i++) {
//         if (control_point[i].type == NULL && i < len_cp - 1) {
//             continue;
//         }
//         unsigned int segment_len = i - start + 1;
//         SliderVector2 segment_vertices[segment_len];
//         for (int i = start; i < segment_len; i++) {
//             segment_vertices[i].x = control_point[i].x;
//             segment_vertices[i].y = control_point[i].y;
//         }
//         SliderType segment_type = control_point[start].type != NULL ? *control_point[start].type : linear;
//         for () {

//         }
//         start = i;
//     }
    
//     // caluclateLength()

//     double d = fmax(0, fmin(progress, 1)) * slider->path.distance;
//     // interpolateVertices(indexOfDisatnce(d), d)

//     return ;
// }

void oos_slider_createnestedhitobjects(Slider *slider) {
    SliderEventDescriptor *slider_event;
    while ((slider_event = oos_slider_generate(slider->start_time, slider->span_duration, slider->velocity, slider->tick_distance, slider->path.distance, slider->span_count, slider->legacy_last_tick_offset)) != NULL) {
        switch (slider_event->type) {
            case sliderevent_tick:
                slider->nested = realloc(slider->nested, (slider->num_nested + 1) * sizeof(*slider->nested));
                // (slider->nested + slider->num_nested)->type = tick;
                // TODO Figure out what `Position = Position + Path.PositionAt(e.PathProgress)` with `slider_event->path_progress`
                // (slider->nested + slider->num_nested)->x = (slider->nested + 0)->x + slider_event->path_progress;
                // (slider->nested + slider->num_nested)->y = ;
                (slider->nested + slider->num_nested)->time = slider_event->time;
                slider->num_nested++;
                break;

            case sliderevent_head:
                break;

            case sliderevent_legacylasttick:
                break;

            case sliderevent_repeat:
                break;

            case sliderevent_tail:
                // Source code just ignores this
                break;
        }
    }
    printf("\n");
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