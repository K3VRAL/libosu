#include "object/std/event.h"

void oos_event_free(Event *event, unsigned int num) {
    for (int i = 0; i < num; i++) {
        if ((event + i)->type == background
            && (event + i)->param.bg.filename != NULL) {
            free((event + i)->param.bg.filename);
        } else if ((event + i)->type == video
            && (event + i)->param.vid.filename != NULL) {
            free((event + i)->param.vid.filename);
        } /* TODO else if ((event + i)->type == storyboard) {
            
        }*/
    }
    free(event);
}