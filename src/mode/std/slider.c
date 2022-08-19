// https://www.geeksforgeeks.org/coroutines-in-c-cpp/
#include "mode/std/slider.h"

SliderEventDescriptor *oms_slider_generate(double start_time, double span_duration, double velocity, double tick_distance, double total_distance, int span_count, double *legacy_last_tick_offset) {
    static int state = 0;

    switch (state) {
        case 0:
            state = 1;
            const double max_length = 100000;
            double length = fmin(max_length, total_distance);
            tick_distance = fmin(tick_distance, fmax(0, length));
            double min_distance_from_end = velocity * 10;

            SliderEventDescriptor *head = malloc(sizeof(SliderEventDescriptor));
            head->type = Head;
            head->span_index = 0;
            head->span_start_time = start_time;
            head->time = start_time;
            head->path_progress = 0;

            return head;
        
        case 1:;
            if (tick_distance != 0) {
                static int span;
                for (span = 0; span < span_count; span++) {
                    double span_start_time = start_time + span * span_duration;
                    bool reversed = span % 2 == 1;
                    SliderEventDescriptor *ticks = oms_slider_generateticks(span, span_start_time, span_duration, reversed, length, tick_distance, min_distance_from_end);
                    // TODO is this relevant?
                    // if (reversed) {
                    //     ticks = ticks.Reversed;
                    // }
                    // TODO is this relevant?
                    // foreach (var e in ticks)
                    //     yield return e;
                    if (span < span_count - 1) {
                        state = 2;
                        SliderEventDescriptor *repeat = malloc(sizeof(SliderEventDescriptor));
                        repeat->type = Repeat;
                        repeat->span_index = span;
                        repeat->span_start_time = start_time + span * span_duration;
                        repeat->time = span_start_time + span_duration;
                        repeat->path_progress = (span + 1) % 2;
                        return repeat;
                    }
        case 2:;
                }
            }
            double total_duration = span_count * span_duration;
            int final_span_index = span_count - 1;
            double final_span_start_time = start_time + final_span_index * span_duration;
            double final_span_end_time = fmax(start_time + total_duration / 2, (final_span_start_time + span_duration) - (legacy_last_tick_offset != NULL ? *legacy_last_tick_offset : 0));
            double final_progress = (final_span_end_time - final_span_start_time) / span_duration;
            if (span_count % 2 == 0) {
                final_progress = 1 - final_progress;
            }
            
            // TODO is this necessary?
            state = 3;
            SliderEventDescriptor *legacy_tick = malloc(sizeof(SliderEventDescriptor));
            legacy_tick->type = LegacyLastTick;
            legacy_tick->span_index = final_span_index;
            legacy_tick->span_start_time = final_span_start_time;
            legacy_tick->time = final_span_end_time;
            legacy_tick->path_progress = final_progress;
            return legacy_tick;
            
        case 3:;
            
            state = 0;
            SliderEventDescriptor *tail = malloc(sizeof(SliderEventDescriptor));
            tail->type = Tail;
            tail->span_index = final_span_index;
            tail->span_start_time = start_time + (span_count - 1) * span_duration;
            tail->time = start_time + total_duration;
            tail->path_progress = (int) span_duration % 2;
            return tail;
    }
    
    return NULL;
}

SliderEventDescriptor *oms_slider_generateticks(int span_index, double span_start_time, double span_duration, bool reversed, double length, double tick_distance, double min_distance_from_end) {
    static int state = 0;
    static SliderEventDescriptor *ticks = NULL;
    switch (state) {
        case 0:
            state = 1;
            ticks = malloc(sizeof(SliderEventDescriptor));
            static double d;
            for (d = tick_distance; d <= length; d += tick_distance) {
                if (d >= length - min_distance_from_end) {
                    break;
                }
                double path_progress = d / length;
                double time_progress = reversed ? 1 - path_progress : path_progress;

                ticks->type = Tick;
                ticks->span_index = span_index;
                ticks->span_start_time = span_start_time;
                ticks->time = span_start_time + time_progress * span_duration;
                ticks->path_progress = path_progress;

                return ticks;
        case 1:;
            }
    }
    state = 0;
    return ticks;
}