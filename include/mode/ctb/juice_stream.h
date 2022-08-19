#ifndef OMC_JUICESTREAM_H
#define OMC_JUICESTREAM_H

#include <math.h>

#include "file/beatmap/difficulty.h"
#include "file/beatmap/timing_points.h"
#include "file/beatmap/hit_objects.h"
#include "mode/std/slider.h"

typedef struct JuiceStream {

} JuiceStream;

JuiceStream *omc_juicestream_nestedhitobjects(Difficulty *, TimingPoint *, HitObject *);
void omc_juicestream_free(JuiceStream *);

#endif