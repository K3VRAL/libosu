#include "file/beatmap.h"

void ofb_hitobject_addfromstring(HitObject *hit_object, char *string) {
	char *copy = strdup(string);
	char *token = strtok(string, ",");
	if (token == NULL) {
		free(copy);
		return;
	}

	hit_object->x = (int) strtol(token, NULL, 10);
	hit_object->y = (int) strtol(strtok(NULL, ","), NULL, 10);
	hit_object->time = (int) strtol(strtok(NULL, ","), NULL, 10);
	hit_object->type = (int) strtol(strtok(NULL, ","), NULL, 10);
	hit_object->hit_sound = (int) strtol(strtok(NULL, ","), NULL, 10);
	switch (hit_object->type) { // Turn every if statement with enum types to switches
		case circle:
		case nc_circle:
			// Why not
			break;

		case slider:
		case nc_slider:
			hit_object->ho.slider.curve_type = strtok(NULL, "|")[0];
			hit_object->ho.slider.curves = NULL;
			hit_object->ho.slider.num_curve = 0;
			hit_object->ho.slider.slides = 0;
			hit_object->ho.slider.length = 0;
			hit_object->ho.slider.edge_sounds = NULL;
			hit_object->ho.slider.num_edge_sound = 0;
			hit_object->ho.slider.edge_sets = NULL;
			hit_object->ho.slider.num_edge_set = 0;
			token = strtok(NULL, ":|,");
			bool in_loop = true;
			while (token != NULL && in_loop) {
				char used_delim = *(copy + (token - string + strlen(token)));
				switch (used_delim) {
					case ':':
						hit_object->ho.slider.curves = realloc(hit_object->ho.slider.curves, (hit_object->ho.slider.num_curve + 1) * sizeof(*hit_object->ho.slider.curves));
						(hit_object->ho.slider.curves + hit_object->ho.slider.num_curve)->x = (int) strtol(token, NULL, 10);
						break;

					case ',':
						in_loop = false;
					case '|':
						(hit_object->ho.slider.curves + hit_object->ho.slider.num_curve++)->y = (int) strtol(token, NULL, 10);
						break;
				}
				if (in_loop) {
					token = strtok(NULL, ":|,");
				}
			}
			hit_object->ho.slider.slides = (int) strtol(strtok(NULL, ","), NULL, 10);
			
			// `length,edgeSounds,edgeSets` can be removed from a slider and still retain the default propertise
			hit_object->ho.slider.edge_sounds = NULL;
			hit_object->ho.slider.edge_sets = NULL;

			if ((token = strtok(NULL, ",")) == NULL) {
				break;
			}
			hit_object->ho.slider.length = strtod(token, NULL);

			if ((token = strtok(NULL, "|,")) == NULL) {
				break;
			}
			hit_object->ho.slider.edge_sounds = NULL;
			hit_object->ho.slider.num_edge_sound = 0;
			in_loop = true;
			while (token != NULL && in_loop) {
				char used_delim = *(copy + (token - string + strlen(token)));
				switch (used_delim) {
					case ',':
						in_loop = false;
					case '|':
						hit_object->ho.slider.edge_sounds = realloc(hit_object->ho.slider.edge_sounds, (hit_object->ho.slider.num_edge_sound + 1) * sizeof(*hit_object->ho.slider.edge_sounds));
						*(hit_object->ho.slider.edge_sounds + hit_object->ho.slider.num_edge_sound++) = (int) strtol(token, NULL, 10);
						break;
				}
				if (in_loop) {
					token = strtok(NULL, "|,");
				}
			}

			if ((token = strtok(NULL, ":|,")) != NULL && *(copy + (token - string + strlen(token))) == ':') {
				hit_object->ho.slider.edge_sets = NULL;
				hit_object->ho.slider.num_edge_set = 0;
				in_loop = true;
				while (token != NULL && in_loop) {
					char used_delim = *(copy + (token - string + strlen(token)));
					switch (used_delim) {
						case ':':
							hit_object->ho.slider.edge_sets = realloc(hit_object->ho.slider.edge_sets, (hit_object->ho.slider.num_edge_set + 1) * sizeof(*hit_object->ho.slider.edge_sets));
							(hit_object->ho.slider.edge_sets + hit_object->ho.slider.num_edge_set)->normal = (int) strtol(token, NULL, 10);
							break;

						case '\0':
						case ',':
							in_loop = false;
						case '|':
							(hit_object->ho.slider.edge_sets + hit_object->ho.slider.num_edge_set++)->additional = (int) strtol(token, NULL, 10);
							break;
					}
					if (in_loop) {
						token = strtok(NULL, ":|,");
					}
				}
			}
			break;

		case spinner:
		case nc_spinner:
			hit_object->ho.spinner.end_time = (int) strtol(strtok(NULL, ","), NULL, 10);
			break;
	}
	free(copy);
	
	// If provided string has `hitSample` section (since it can be defaulted)
	hit_object->hit_sample.filename = NULL;
	hit_object->hit_sample.normal_set = 0;
	hit_object->hit_sample.addition_set = 0;
	hit_object->hit_sample.index = 0;
	hit_object->hit_sample.volume = 0;
	
	if ((token = strtok(NULL, ":")) == NULL) {
		return;
	}
	hit_object->hit_sample.normal_set = (int) strtol(token, NULL, 10);
	hit_object->hit_sample.addition_set = (int) strtol(strtok(NULL, ":"), NULL, 10);
	hit_object->hit_sample.index = (int) strtol(strtok(NULL, ":"), NULL, 10);
	hit_object->hit_sample.volume = (int) strtol(strtok(NULL, ":"), NULL, 10);
	if ((token = strtok(NULL, ":")) != NULL) {
		hit_object->hit_sample.filename = strdup(token);
	}
}

void ofb_hitobject_tostring(char **output, HitObject hit_object) {
	int size_x = ou_comparing_size(hit_object.x);
	int size_y = ou_comparing_size(hit_object.y);
	int size_time = ou_comparing_size(hit_object.time);
	int size_type = ou_comparing_size(hit_object.type);
	int size_hitsound = ou_comparing_size(hit_object.hit_sound);
	int size_total = size_x + 1 + size_y + 1 + size_time + 1 + size_type + 1 + size_hitsound;
	int len = size_total + (1 + 1);
	*output = calloc(len, sizeof(**output));
	snprintf(*output, len, "%d,%d,%d,%d,%d",
		hit_object.x,
		hit_object.y,
		hit_object.time,
		hit_object.type,
		hit_object.hit_sound
	);
	switch (hit_object.type) {
		case circle:
		case nc_circle:
			break;

		case slider:
		case nc_slider: {
			int len_buffer = 1;
			char *buffer = calloc(len_buffer, sizeof(*buffer));
			
			{
				len_buffer += 1 + 1;
				buffer = realloc(buffer, len_buffer * sizeof(*buffer));
				snprintf(buffer, len_buffer, ",%c", hit_object.ho.slider.curve_type);
			}
			
			for (int j = 0; j < hit_object.ho.slider.num_curve; j++) {
				int len_buffer_x = ou_comparing_size((hit_object.ho.slider.curves + j)->x);
				int len_buffer_y = ou_comparing_size((hit_object.ho.slider.curves + j)->y);
				int len_buffer_curve = 1 + len_buffer_x + 1 + len_buffer_y + 1;
				char *buffer_curve = calloc(len_buffer_curve, sizeof(*buffer_curve));
				snprintf(buffer_curve, len_buffer_curve, "|%d:%d", (hit_object.ho.slider.curves + j)->x, (hit_object.ho.slider.curves + j)->y);

				len_buffer += len_buffer_curve;
				buffer = realloc(buffer, len_buffer * sizeof(*buffer));
				strcat(buffer, buffer_curve);
				free(buffer_curve);
			}

			{
				int len_buffer_slides = ou_comparing_size(hit_object.ho.slider.slides);
				int len_buffer_length = ou_comparing_size((int) hit_object.ho.slider.length);
				int trailing_zeros = ou_comparing_trailing(hit_object.ho.slider.length, len_buffer_length);
				int len_buffer_sl = 1 + len_buffer_slides + 1 + (len_buffer_length + (trailing_zeros != 0 ? 1 : 0) + trailing_zeros + 1);
				char *buffer_sl = calloc(len_buffer_sl, sizeof(*buffer_sl));
				snprintf(buffer_sl, len_buffer_sl, ",%d,%.*f", hit_object.ho.slider.slides, trailing_zeros, hit_object.ho.slider.length);

				len_buffer += len_buffer_sl;
				buffer = realloc(buffer, len_buffer * sizeof(*buffer));
				strcat(buffer, buffer_sl);
				free(buffer_sl);
			}

			if (hit_object.ho.slider.edge_sounds != NULL || hit_object.ho.slider.num_edge_sound > 0) {
				for (int j = 0; j < hit_object.ho.slider.num_edge_sound; j++) {
					int len_buffer_edge_sound = ou_comparing_size(*(hit_object.ho.slider.edge_sounds + j));
					int len_edge_sound = 1 + len_buffer_edge_sound + 1;
					char *buffer_edge_sound = calloc(len_edge_sound, sizeof(*buffer_edge_sound));
					snprintf(buffer_edge_sound, len_edge_sound, (j != 0 ? "|%d" : ",%d"), *(hit_object.ho.slider.edge_sounds + j));

					len_buffer += len_edge_sound;
					buffer = realloc(buffer, len_buffer * sizeof(*buffer));
					strcat(buffer, buffer_edge_sound);
					free(buffer_edge_sound);
				}
			}

			if (hit_object.ho.slider.edge_sounds != NULL || hit_object.ho.slider.num_edge_sound > 0) {
				for (int j = 0; j < hit_object.ho.slider.num_edge_set; j++) {
					int len_buffer_edge_set_normal = ou_comparing_size((hit_object.ho.slider.edge_sets + j)->normal);
					int len_buffer_edge_set_additional = ou_comparing_size((hit_object.ho.slider.edge_sets + j)->additional);
					int len_edge_set = 1 + len_buffer_edge_set_normal + 1 + len_buffer_edge_set_additional + 1;
					char *buffer_edge_set = calloc(len_edge_set, sizeof(*buffer_edge_set));
					snprintf(buffer_edge_set, len_edge_set, (j != 0 ? "|%d:%d" : ",%d:%d"), (hit_object.ho.slider.edge_sets + j)->normal, (hit_object.ho.slider.edge_sets + j)->additional);
				
					len_buffer += len_edge_set;
					buffer = realloc(buffer, len_buffer * sizeof(*buffer));
					strcat(buffer, buffer_edge_set);
					free(buffer_edge_set);
				}
			}

			len += len_buffer;
			*output = realloc(*output, len * sizeof(**output));
			strcat(*output, buffer);
			free(buffer);
			break;
		}

		case spinner:
		case nc_spinner: {
			int size_spinner = ou_comparing_size(hit_object.ho.spinner.end_time);
			int len_buffer = 1 + size_spinner + 1;
			char *buffer = calloc(len_buffer, sizeof(*buffer));
			snprintf(buffer, len_buffer, ",%d", hit_object.ho.spinner.end_time);

			len += len_buffer;
			*output = realloc(*output, len * sizeof(**output));
			strcat(*output, buffer);
			free(buffer);
			break;
		}
	}

	if (hit_object.hit_sample.normal_set != 0 ||
		hit_object.hit_sample.addition_set != 0 ||
		hit_object.hit_sample.index != 0 ||
		hit_object.hit_sample.volume != 0 ||
		hit_object.hit_sample.filename != NULL) {
		int size_normalset = ou_comparing_size(hit_object.hit_sample.normal_set);
		int size_additionset = ou_comparing_size(hit_object.hit_sample.addition_set);
		int size_index = ou_comparing_size(hit_object.hit_sample.index);
		int size_volume = ou_comparing_size(hit_object.hit_sample.volume);
		int size_filename = (hit_object.hit_sample.filename != NULL ? strlen(hit_object.hit_sample.filename) : 0);
		int len_buffer = 1 + size_normalset + 1 + size_additionset + 1 + size_index + 1 + size_volume + 1 + size_filename + 1;
		char *buffer = calloc(len_buffer, sizeof(*buffer));
		snprintf(buffer, len_buffer, ",%d:%d:%d:%d:%s",
			hit_object.hit_sample.normal_set,
			hit_object.hit_sample.addition_set,
			hit_object.hit_sample.index,
			hit_object.hit_sample.volume,
			hit_object.hit_sample.filename != NULL ? hit_object.hit_sample.filename : ""
		);

		len += len_buffer;
		*output = realloc(*output, len * sizeof(**output));
		strcat(*output, buffer);
		free(buffer);
	}
	strcat(*output, "\n");
}