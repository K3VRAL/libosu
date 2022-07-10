#ifndef EVENTS_H
#define EVENTS_H

// TODO

// typedef struct E {

// } E;

// typedef enum EType {
//     background,
//     video,
//     bbreak,
//     storyboard
// } EType;

// typedef struct Event {
//     EType event_type;
//     E event;
// } Event;

typedef struct Events {
    // Event *events;
    // unsigned int num;
} Events;

Events osuv_init();
void osuv_free(Events *);
void osuv_set(Events *, char *);

#endif