#ifndef OMC_JUICESTREAM_H
#define OMC_JUICESTREAM_H

#include <math.h>

#include "file/beatmap/hit_objects.h"

typedef struct JuiceStream {

} JuiceStream;

JuiceStream omc_juicestream_generate(HitObject hit_object);
void omc_juicestream_free(JuiceStream *js);

#endif