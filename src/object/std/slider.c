// https://www.geeksforgeeks.org/coroutines-in-c-cpp/
#include "object/std.h"

// TODO is all functions completed and memory-safe?
// Slider oos_slider_init(Difficulty difficulty, TimingPoint *timing_point, unsigned int num, HitObject hit_object) {
//     TimingPoint tp = oos_timingpoint_attime(hit_object.time, timing_point, num);
//     double scoring_distance = oos_hitobject_BASESCORINGDISTANCE * difficulty.slider_multiplier * /* TODO find out `DifficultyControlPoint.SliderVelocity` */;
//     Slider slider = {
//         .start_time = hit_object.time,
//         .end_time = slider.start_time + /* TODO find out `SpanCount()` */ * slider.path.distance,
//         .duration = slider.end_time - slider.start_time,
//         .path = ,
//         .distance = ,
//         .legacy_last_tick_offset = ,
//         .span_duration = ,
//         .tick_distance_multiplier = 1,
//         .velocity = scoring_distance / tp.beat_length,
//         .tick_distance = scoring_distance / difficulty.slider_tick_rate * slider.tick_distance_multiplier,
//     };
//     return slider;
// }

void oos_slider_createnestedhitobjects(Slider slider) {
    SliderEventDescriptor *slider_event;
    while ((slider_event = oos_slider_generate(slider.start_time, slider.span_duration, slider.velocity, slider.tick_distance, slider.path.distance, slider.span_count, slider.legacy_last_tick_offset)) != NULL) {
        switch (slider_event->type) {
            case tick:
                printf("Tick, ");
                break;

            case head:
                printf("Head, ");
                break;

            case legacy_last_tick:
                printf("Legacy Last Tick, ");
                break;

            case repeat:
                printf("Repeat, ");
                break;

            case tail:
                printf("Tail (nope), ");
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
                object = malloc(sizeof(SliderEventDescriptor));
                object->type = head;
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
                        object = malloc(sizeof(SliderEventDescriptor));
                        object->type = repeat;
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
                object = malloc(sizeof(SliderEventDescriptor));
                object->type = legacy_last_tick;
                object->span_index = final_span_index;
                object->span_start_time = final_span_start_time;
                object->time = final_span_end_time;
                object->path_progress = final_progress;
                return object;
            }
            
        case 4:
            state = 5;
            object = malloc(sizeof(SliderEventDescriptor));
            object->type = tail;
            object->span_index = final_span_index;
            object->span_start_time = start_time + (span_count - 1) * span_duration;
            object->time = start_time + total_duration;
            object->path_progress = (int) span_duration % 2;
            return object;

        case 5:;
    }
    
    state = 0;
    object = NULL;
    return NULL;
}

SliderEventDescriptor *oos_slider_generateticks(int span_index, double span_start_time, double span_duration, bool reversed, double length, double tick_distance, double min_distance_from_end) {
    static int state = 0;
    static SliderEventDescriptor *object = NULL;
    if (object != NULL) {
        free(object);
    }
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

                    object = malloc(sizeof(SliderEventDescriptor));
                    object->type = tick;
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

                    object = malloc(sizeof(SliderEventDescriptor));
                    object->type = tick;
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
    object = NULL;
    return NULL;
}