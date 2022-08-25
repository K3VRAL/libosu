#ifndef OFB_EVENT_H
#define OFB_EVENT_H

#include "object/std/event.h"

Event *ofb_event_addfromstring(char *);
char *ofb_event_tostring(Event);

#endif