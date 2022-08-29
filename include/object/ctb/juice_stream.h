#ifndef OOC_JUICESTREAM_H
#define OOC_JUICESTREAM_H

#include "object/std.h"

typedef struct JuiceStream {

} JuiceStream;

JuiceStream *ooc_juicestream_nestedhitobjects(Difficulty *, TimingPoint *, HitObject *);
void ooc_juicestream_free(JuiceStream *);

#endif