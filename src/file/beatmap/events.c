#include "file/beatmap/events.h"

void ofb_events_set(Event **events, unsigned int *num, char *key_value_pair) {
}

void ofb_events_free(Event *events) {
}

void ofb_events_tofile(Event *events, unsigned int num, FILE *fp) {
    fputs("[Events]\n", fp);
    fputs("\n", fp);
}