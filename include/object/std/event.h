#ifndef OOS_EVENT_H
#define OOS_EVENT_H

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "unrelated/comparing.h"

typedef struct EVBackground {
    char *filename;
    int x_offset;
    int y_offset;
} EVBackground;

typedef EVBackground EVVideo;

typedef struct EVBreak {
    int end_time;
} EVBreak;

typedef struct EVStoryboard {
    // TODO
} EVStoryboard;

typedef union EVParam {
    EVBackground bg;
    EVVideo vid;
    EVBreak brk;
    EVStoryboard sb;
} EVParam;

typedef enum EVTYPE {
    background = 0,
    video = 1,
    breaks = 2,
    storyboard/* TODO = */
} EVType;

typedef struct Event {
    EVType type;
    int start_time;
    EVParam param;
} Event;

void oos_event_free(Event *, unsigned int num);

#endif