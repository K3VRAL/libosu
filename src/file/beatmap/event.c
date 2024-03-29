#include "file/beatmap.h"

void ofb_event_addfromstring(Event *event, char *string) {
	char *token = strtok(string, ",");
	if (token == NULL) {
		return;
	}
	
	if (strcmp("Video", token) == 0) {
		event->type = 1;
	} else if (strcmp("Break", token) == 0) {
		event->type = 2;
	} else {
		event->type = (int) strtol(token, NULL, 10);
	}
	event->start_time = (int) strtol(strtok(NULL, ","), NULL, 10);
	switch (event->type) {
		case background: {
			event->param.bg.filename = strdup(strtok(NULL, "\""));
			char *x = strtok(NULL, ",");
			if (x == NULL) {
				break;
			}
			event->param.bg.x_offset = (int) strtol(x, NULL, 10);
			event->param.bg.y_offset = (int) strtol(strtok(NULL, ","), NULL, 10);
			break;
		}
		
		case video: // In case they update this
			event->param.vid.filename = strdup(strtok(NULL, "\""));
			event->param.vid.x_offset = strtok(NULL, ",") != NULL ? (int) strtol(strtok(NULL, ","), NULL, 10) : 0;
			event->param.vid.y_offset = strtok(NULL, ",") != NULL ? (int) strtol(strtok(NULL, ","), NULL, 10) : 0;
			break;

		case breaks:
			event->param.brk.end_time = (int) strtol(strtok(NULL, ","), NULL, 10);
			break;

		case storyboard:
			break;
	}
}

void ofb_event_tostring(char **output, Event event) {
	int size_type = ou_comparing_size(event.type);
	int size_start_time = ou_comparing_size(event.start_time);
	int size_total = size_type + 1 + size_start_time;
	int len = size_total + (1 + 1);
	*output = calloc(len, sizeof(**output));
	snprintf(*output, len, "%d,%d", event.type, event.start_time);
	switch (event.type) {
		case background: {
			int size_filename = strlen(event.param.bg.filename);
			int size_x_offset = ou_comparing_size(event.param.bg.x_offset);
			int size_y_offset = ou_comparing_size(event.param.bg.y_offset);
			int len_buffer = 1 + 1 + size_filename + 1 + 1 + size_x_offset + 1 + size_y_offset + 1;
			char *buffer = calloc(len_buffer, sizeof(*buffer));
			snprintf(buffer, len_buffer, ",\"%s\",%d,%d", event.param.bg.filename, event.param.bg.x_offset, event.param.bg.y_offset);

			len += len_buffer;
			*output = realloc(*output, len * sizeof(**output));
			strcat(*output, buffer);
			free(buffer);
			break;
		}

		case video: {
			int size_filename = strlen(event.param.vid.filename);
			int size_x_offset = ou_comparing_size(event.param.vid.x_offset);
			int size_y_offset = ou_comparing_size(event.param.vid.y_offset);
			int len_buffer = 1 + 1 + size_filename + 1 + 1 + size_x_offset + 1 + size_y_offset + 1;
			char *buffer = calloc(len_buffer, sizeof(*buffer));
			snprintf(buffer, len_buffer, ",\"%s\",%d,%d", event.param.vid.filename, event.param.vid.x_offset, event.param.vid.y_offset);

			len += len_buffer;
			*output = realloc(*output, len * sizeof(**output));
			strcat(*output, buffer);
			free(buffer);
			break;
		}
			
		case breaks: {
			int size_end_time = ou_comparing_size(event.param.brk.end_time);
			int len_buffer = 1 + size_end_time;
			char *buffer = calloc(len_buffer, sizeof(*buffer));
			snprintf(buffer, len_buffer, ",%d", event.param.brk.end_time);

			len += len_buffer;
			*output = realloc(*output, len * sizeof(**output));
			strcat(*output, buffer);
			free(buffer);
			break;
		}
			
		case storyboard:
			break;
	}
	strcat(*output, "\n");
}
