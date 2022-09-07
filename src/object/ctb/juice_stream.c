#include "object/ctb.h"

void ooc_juicestream_init(CatchHitObject **object, HitObject hit_object) {
    if (!(hit_object.type == slider || hit_object.type == nc_slider)) {
        return;
    }
    ooc_hitobject_init(object, hit_object.time, hit_object.x, 0);
    (*object)->type = juice_stream;
}

// https://github.com/ppy/osu/blob/master/osu.Game.Rulesets.Catch/Beatmaps/CatchBeatmapProcessor.cs
// https://github.com/ppy/osu/blob/master/osu.Game.Rulesets.Catch/Objects/JuiceStream.cs
// https://github.com/ppy/osu/blob/38064165413eb919b9a32433d8be5051fc1215f7/osu.Game/Rulesets/Objects/SliderEventGenerator.cs
// JuiceStream *ooc_juicestream_createnestedjuice(CatchHitObject *object, Difficulty diff, TimingPoint tp) {
//     // TODO add `if` to check if `CatchHitObject` is the specific object (then do this with every single object)
//     if (object->type != juice_stream) {
//         return NULL;
//     }

//     int node_index = 0;
//     SliderEventDescriptor *last_event = NULL;
//     SliderEventDescriptor *e;
//     while ((e = oos_slider_generate(object->start_time, , , , , , )) != NULL) {
//         if (last_event != NULL) {
//             double since_last_tick = e->time - last_event->time;
//             if (since_last_tick > 80) {
//                 double time_between_tiny = since_last_tick;
//                 while (time_between_tiny > 100) {
//                     time_between_tiny /= 2;
//                 }
//                 for (double t = time_between_tiny; t < since_last_tick; t += time_between_tiny) {
//                     // TODO AddNested
//                     object->cho.js.
//                 }
//             }
//         }

//         last_event = e;
//         switch (e->type) {
//             case tick:
                
//                 break;

//             case head:
//             case tail:
//             case repeat:
//                 break;

//             case legacy_last_tick:
//                 break;
//         }
//     }
// }

// void ooc_juicestream_free(JuiceStream *js) {

// }