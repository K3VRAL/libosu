#include "object/std.h"

void oos_event_free(Event *event, unsigned int num) {
    for (int i = 0; i < num; i++) {
        switch ((event + i)->type) {
            case background:
                if ((event + i)->param.bg.filename != NULL) {
                    free((event + i)->param.bg.filename);
                }
                break;
            
            case video:
                if ((event + i)->param.vid.filename != NULL) {
                    free((event + i)->param.vid.filename);
                }
                break;

            case breaks:
                break;

            case storyboard:
                // TODO
                break;
        }
    }
    free(event);
}