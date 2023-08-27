#include "object/std.h"

void oos_event_free(Event event) {
	switch (event.type) {
		case background:
			if (event.param.bg.filename != NULL) {
				free(event.param.bg.filename);
			}
			break;
		
		case video:
			if (event.param.vid.filename != NULL) {
				free(event.param.vid.filename);
			}
			break;

		case breaks:
			break;

		case storyboard:
			break;
	}
}

void oos_event_freebulk(Event *event, unsigned int num) {
	if (event == NULL) {
		return;
	}
	for (int i = 0; i < num; i++) {
		oos_event_free(*(event + i));
	}
	free(event);
}