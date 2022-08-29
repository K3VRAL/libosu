#include "object/ctb.h"

CatchHitObject *ooc_juicestream_init(HitObject hit_object) {
    if (!(hit_object.type == slider || hit_object.type == nc_slider)) {
        return NULL;
    }
    CatchHitObject *object = ooc_hitobject_init(hit_object.time, hit_object.x, 0);
    object->type = juice_stream;
    return object;
}

// https://github.com/ppy/osu/blob/master/osu.Game.Rulesets.Catch/Beatmaps/CatchBeatmapProcessor.cs
// https://github.com/ppy/osu/blob/master/osu.Game.Rulesets.Catch/Objects/JuiceStream.cs
// https://github.com/ppy/osu/blob/38064165413eb919b9a32433d8be5051fc1215f7/osu.Game/Rulesets/Objects/SliderEventGenerator.cs
// JuiceStream *ooc_juicestream_nestedhitobjects(Difficulty *diff, TimingPoint *tp, HitObject *ho) {
//     // int droplet_samples = // TODO
//     int node_index = 0;
//     SliderEventDescriptor *last_event = NULL;
//     for (SliderEventDescriptor *i = oos_slider_generate(ho->time, , , , , , ); ; ) {

//     }
// }

// void ooc_juicestream_free(JuiceStream *js) {

// }