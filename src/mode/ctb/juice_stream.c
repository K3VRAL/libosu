#include "mode/ctb/juice_stream.h"

// https://github.com/ppy/osu/blob/master/osu.Game.Rulesets.Catch/Beatmaps/CatchBeatmapProcessor.cs
// https://github.com/ppy/osu/blob/master/osu.Game.Rulesets.Catch/Objects/JuiceStream.cs
// https://github.com/ppy/osu/blob/38064165413eb919b9a32433d8be5051fc1215f7/osu.Game/Rulesets/Objects/SliderEventGenerator.cs
JuiceStream *omc_juicestream_nestedhitobjects(Difficulty *diff, TimingPoint *tp, HitObject *ho) {
    // int droplet_samples = // TODO
    int node_index = 0;
    SliderEventDescriptor *last_event = NULL;
    for (SliderEventDescriptor *i = oms_slider_generate(ho->time, , , , , , ); ; ) {

    }
}

void omc_juicestream_free(JuiceStream *js) {

}