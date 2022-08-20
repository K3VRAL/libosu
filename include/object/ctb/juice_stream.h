#ifndef OOC_JUICESTREAM_H
#define OOC_JUICESTREAM_H

#include <math.h>

#include "file/beatmap/difficulty.h"
#include "file/beatmap/timing_point.h"
#include "file/beatmap/hit_object.h"
#include "object/std/slider.h"

typedef struct JuiceStream {

} JuiceStream;

JuiceStream *ooc_juicestream_nestedhitobjects(Difficulty *, TimingPoint *, HitObject *);
void ooc_juicestream_free(JuiceStream *);

#endif