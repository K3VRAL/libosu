#ifndef OOC_HITOBJECT_H
#define OOC_HITOBJECT_H

#include <stdlib.h>

#include "object/std.h"
#include "banana_shower.h"
#include "fruit.h"
#include "juice_stream.h"

typedef union CHO {
	Fruit *f;
	JuiceStream *js;
	BananaShower *bs;
	Banana *b;
	Droplet *d;
	TinyDroplet *td;
} CHO;

typedef enum CHOType {
	catchhitobject_fruit,
	catchhitobject_juicestream,
	catchhitobject_bananashower,
	catchhitobject_banana,
	catchhitobject_droplet,
	catchhitobject_tinydroplet
} CHOType;

typedef struct CatchHitObject {
	float start_time;
	float x;
	float x_offset;
	CHOType type;
	CHO cho;
	HitObject *refer;
} CatchHitObject;

extern const float ooc_hitobject_OBJECTRADIUS;

void ooc_hitobject_init(CatchHitObject *, float, float, float, HitObject *);
void ooc_hitobject_free(CatchHitObject);
void ooc_hitobject_freebulk(CatchHitObject *, unsigned int);
void ooc_hitobject_freebulkrefer(CatchHitObject *, unsigned int);

#endif