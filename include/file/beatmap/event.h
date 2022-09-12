#ifndef OFB_EVENT_H
#define OFB_EVENT_H

#include "object/std/event.h"

/*
    Converts from a string to an Event object

    argsreturn
        Event *
    args
        char *
*/
void ofb_event_addfromstring(Event *, char *);

/*
    Converts from a Event object to a string

    argsreturn
        char *
    args
        Event
*/
void ofb_event_tostring(char *, Event);

#endif