// https://www.geeksforgeeks.org/coroutines-in-c-cpp/
#include "object/std.h"

Slider *oos_slider_init(Difficulty difficulty, InheritedTimingPoint *inherited, UninheritedTimingPoint *uninherited, HitObject hit_object) {
    TimingPoint tp_inherited = oos_timingpoint_attime(hit_object.time, inherited->tp, inherited->num);
    TimingPoint tp_uninherited = oos_timingpoint_attime(hit_object.time, uninherited->tp, uninherited->num);
    double scoring_distance = oos_hitobject_BASESCORINGDISTANCE * difficulty.slider_multiplier * (-100 / tp_inherited.beat_length);
    Slider *slider = malloc(sizeof(Slider));
    slider->tick_distance_multiplier = 1;
    slider->velocity = scoring_distance / tp_uninherited.beat_length;
    slider->tick_distance = scoring_distance / difficulty.slider_tick_rate * slider->tick_distance_multiplier;
    slider->path.distance = hit_object.ho.slider.length;
    slider->start_time = hit_object.time;
    slider->span_count = hit_object.ho.slider.slides;
    slider->end_time = slider->start_time + slider->span_count * slider->path.distance / slider->velocity;
    slider->duration = slider->end_time - slider->start_time;
    slider->span_duration = slider->duration / slider->span_count;
    slider->legacy_last_tick_offset = malloc(sizeof(double));
    *slider->legacy_last_tick_offset = 36;
    slider->nested = NULL; // TODO figure out a way to free all of this
    slider->num_nested = 0;
    return slider;
}

Slider *oos_slider_initwouninandinherited(Difficulty difficulty, TimingPoint *timing_point, unsigned int num, HitObject hit_object) {
    InheritedTimingPoint *inherited = oos_inheritedpoint_init(timing_point, num);
    UninheritedTimingPoint *uninherited = oos_uninheritedpoint_init(timing_point, num);
    Slider *slider = oos_slider_init(difficulty, inherited, uninherited, hit_object);
    oos_inheritedpoint_free(inherited);
    oos_uninheritedpoint_free(uninherited);
    return slider;
}

// TODO do it in this format for every other freeable object
void oos_slider_free(Slider *slider) {
    if (slider == NULL) {
        return;
    }
    if (slider->legacy_last_tick_offset != NULL) {
        free(slider->legacy_last_tick_offset);
    }
    free(slider);
}

void oos_slider_createnestedhitobjects(Slider *slider) {
    SliderEventDescriptor *slider_event;
    while ((slider_event = oos_slider_generate(slider->start_time, slider->span_duration, slider->velocity, slider->tick_distance, slider->path.distance, slider->span_count, slider->legacy_last_tick_offset)) != NULL) {
        // TODO
        switch (slider_event->type) {
            case tick:
                slider->nested = realloc(slider->nested, (slider->num_nested + 1) * sizeof(HitObject));
                slider->num_nested++;
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
                printf("Tail, ");
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
    return NULL;
}