#ifndef OFB_EVENT_H
#define OFB_EVENT_H

#include <stdio.h>
#include <stdlib.h>

// TODO

// typedef struct E {

// } E;

// typedef enum EType {
//     background,
//     video,
//     bbreak,
//     storyboard
// } EType;

typedef struct Event {
    // EType event_type;
    // E event;
} Event;

void ofb_event_setfromstring(Event *, char *);
void ofb_event_free(Event *);
char *ofb_event_tostring(Event);

#endif