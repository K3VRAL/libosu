#ifndef OFB_EVENTS_H
#define OFB_EVENTS_H

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

void ofb_events_free(Event **);
void ofb_events_set(Event **, unsigned int *, char *);

#endif