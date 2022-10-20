// https://www.geeksforgeeks.org/coroutines-in-c-cpp/
#include "object/std.h"

void oos_slider_init(Slider *slider, Difficulty difficulty, TimingPoint tp_inherited, TimingPoint tp_uninherited, HitObject hit_object) {
	slider->legacy_last_tick_offset = NULL;
	oos_slider_calculateslider(slider, difficulty, tp_inherited, tp_uninherited, hit_object);

	slider->ho_data = calloc(1, sizeof(*slider->ho_data));
	slider->ho_data->curve_type = hit_object.ho.slider.curve_type;
	slider->ho_data->num_curve = hit_object.ho.slider.num_curve;
	slider->ho_data->curves = calloc(slider->ho_data->num_curve, sizeof(*slider->ho_data->curves));
	for (int i = 0; i < slider->ho_data->num_curve; i++) {
		(slider->ho_data->curves + i)->x = (hit_object.ho.slider.curves + i)->x;
		(slider->ho_data->curves + i)->y = (hit_object.ho.slider.curves + i)->y;
	}

	slider->calculate_path = NULL;
	slider->calculatepath_len = 0;
	slider->cumulative_length = NULL;
	slider->cumulativelength_len = 0;
	slider->control_point = NULL;
	slider->controlpoint_len = 0;

	slider->nested = NULL;
	slider->num_nested = 0;
}

void oos_slider_initwtp(Slider *slider, Difficulty difficulty, TimingPoint *timing_point, unsigned int num, HitObject hit_object) {
	InheritedTimingPoint inherited;
	oos_inheritedpoint_init(&inherited, timing_point, num);

	UninheritedTimingPoint uninherited;
	oos_uninheritedpoint_init(&uninherited, timing_point, num);
	
	oos_slider_initwherited(slider, difficulty, inherited, uninherited, hit_object);
	
	oos_inheritedpoint_free(inherited);
	oos_uninheritedpoint_free(uninherited);
}

void oos_slider_initwherited(Slider *slider, Difficulty difficulty, InheritedTimingPoint inherited, UninheritedTimingPoint uninherited, HitObject hit_object) {
	TimingPoint tp_inherited;
	oos_timingpoint_attime(&tp_inherited, hit_object.time, inherited.tp, inherited.num);
	
	TimingPoint tp_uninherited;
	oos_timingpoint_attime(&tp_uninherited, hit_object.time, uninherited.tp, uninherited.num);
	
	oos_slider_init(slider, difficulty, tp_inherited, tp_uninherited, hit_object);
}

void oos_slider_calculateslider(Slider *slider, Difficulty difficulty, TimingPoint tp_inherited, TimingPoint tp_uninherited, HitObject hit_object) {
	double scoring_distance = oos_hitobject_BASESCORINGDISTANCE * difficulty.slider_multiplier * (-100 / tp_inherited.beat_length);
	slider->tick_distance_multiplier = 1;
	slider->velocity = scoring_distance / tp_uninherited.beat_length;
	slider->tick_distance = scoring_distance / difficulty.slider_tick_rate * slider->tick_distance_multiplier;
	slider->path.distance = hit_object.ho.slider.length;
	slider->start_time = hit_object.time;
	slider->span_count = hit_object.ho.slider.slides;
	slider->end_time = slider->start_time + slider->span_count * slider->path.distance / slider->velocity;
	slider->duration = slider->end_time - slider->start_time;
	slider->span_duration = slider->duration / slider->span_count;
	if (slider->legacy_last_tick_offset != NULL) {
		free(slider->legacy_last_tick_offset);
	}
	slider->legacy_last_tick_offset = calloc(1, sizeof(*slider->legacy_last_tick_offset));
	*slider->legacy_last_tick_offset = 36;
	slider->start_position.x = hit_object.x;
	slider->start_position.y = hit_object.y;
}

void oos_slider_free(Slider slider) {
	if (slider.legacy_last_tick_offset != NULL) {
		free(slider.legacy_last_tick_offset);
	}
	if (slider.ho_data != NULL) {
		oos_hitobject_freeslider(*slider.ho_data);
		free(slider.ho_data);
	}
	if (slider.calculate_path != NULL) {
		free(slider.calculate_path);
	}
	if (slider.cumulative_length != NULL) {
		free(slider.cumulative_length);
	}
	if (slider.control_point != NULL) {
		free(slider.control_point);
	}
	if (slider.nested != NULL) {
		free(slider.nested);
	}
}

void oos_slider_linearapproximate(SliderVector2 **result, unsigned int *len_result, SliderVector2 *vertices, unsigned int len) {
	*result = calloc(len, sizeof(**result));
	for (int i = 0; i < len; i++) {
		(*result + i)->type = NULL;
		(*result + i)->x = (vertices + i)->x;
		(*result + i)->y = (vertices + i)->y;
		*len_result = len;
	}
}

void oos_slider_catmullfindpoint(SliderVector2 *result, SliderVector2 *v1, SliderVector2 *v2, SliderVector2 *v3, SliderVector2 *v4, float t) {
	float t2 = t * t;
	float t3 = t * t2;
	result->x = 0.5 * (2 * v2->x + (-v1->x + v3->x) * t + (2 * v1->x - 5 * v2->x + 4 * v3->x - v4->x) * t2 + (-v1->x + 3 * v2->x - 3 * v3->x + v4->x) * t3);
	result->y = 0.5 * (2 * v2->y + (-v1->y + v3->y) * t + (2 * v1->y - 5 * v2->y + 4 * v3->y - v4->y) * t2 + (-v1->y + 3 * v2->y - 3 * v3->y + v4->y) * t3);
}

void oos_slider_catmullapproximate(SliderVector2 **result, unsigned int *len_result, SliderVector2 *vertices, unsigned int len) {
	int catmull_detail = 50;
	int catmull_len = (len - 1) * catmull_detail * 2;
	*result = calloc(catmull_len, sizeof(**result));
	for (int i = 0; i < len - 1; i++) {
		SliderVector2 v1;
		if (i > 0) {
			v1 = *(vertices + i - 1);
		} else {
			v1 = *(vertices + i);
		}
		
		SliderVector2 v2 = *(vertices + i);
		SliderVector2 v3;
		if (i < len - 1) {
			v3 = *(vertices + i + 1);
		} else {
			v3.x = v2.x + v2.x - v1.x;
			v3.y = v2.y + v2.y - v1.y;
		}
		SliderVector2 v4;
		if (i < len - 2) {
			v4 = *(vertices + i + 2);
		} else {
			v4.x = v3.x + v3.x - v2.x;
			v4.y = v3.y + v3.y - v2.y;
		}

		for (int c = 0; c < catmull_detail; c++) {
			{
				SliderVector2 catmull_result;
				oos_slider_catmullfindpoint(&catmull_result, &v1, &v2, &v3, &v4, (float) c / catmull_detail);
				(*result + *len_result)->x = catmull_result.x;
				(*result + *len_result)->y = catmull_result.y;
				(*len_result)++;
			}
			{
				SliderVector2 catmull_result;
				oos_slider_catmullfindpoint(&catmull_result, &v1, &v2, &v3, &v4, (float) (c + 1) / catmull_detail);
				(*result + *len_result)->x = catmull_result.x;
				(*result + *len_result)->y = catmull_result.y;
				(*len_result)++;
			}
		}
	}
}

void oos_slider_approximatecirculararc(SliderVector2 **result, unsigned int *len_result, SliderVector2 *vertices, unsigned int len) {
	// circularArcProperties
	SliderVector2 a = *(vertices + 0);
	SliderVector2 b = *(vertices + 1);
	SliderVector2 c = *(vertices + 2);
	const float float_epsilon = 1e-3f;
	if (fabs(0 - ((b.y - a.y) * (c.x - a.x) - (b.x - a.x) * (c.y - a.y))) <= float_epsilon) {
		oos_slider_approximatebezier(result, len_result, vertices, len);
		return;
	}
	float d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
	float a_sq = sqrtf(a.x * a.x + a.y * a.y);
	a_sq *= a_sq;
	float b_sq = sqrtf(b.x * b.x + b.y * b.y);
	b_sq *= b_sq;
	float c_sq = sqrtf(c.x * c.x + c.y * c.y);
	c_sq *= c_sq;
	SliderVector2 centre = {
		.x = (a_sq * (b.y - c.y) + b_sq * (c.y - a.y) + c_sq * (a.y - b.y)) / d,
		.y = (a_sq * (c.x - b.x) + b_sq * (a.x - c.x) + c_sq * (b.x - a.x)) / d
	};
	SliderVector2 d_a = {
		.x = a.x - centre.x,
		.y = a.y - centre.y
	};
	SliderVector2 d_c = {
		.x = c.x - centre.x,
		.y = c.y - centre.y
	};
	float radius = sqrtf(d_a.x * d_a.x + d_a.y * d_a.y);
	double theta_start = atan2(d_a.y, d_a.x);
	double theta_end = atan2(d_c.y, d_c.x);
	while (theta_end < theta_start) {
		theta_end += 2 * M_PI;
	}
	double direction = 1;
	double theta_range = theta_end - theta_start;
	SliderVector2 ortho_a_to_c = {
		.x = c.x - a.x,
		.y = c.y - a.y
	};
	float temp = ortho_a_to_c.x;
	ortho_a_to_c.x = ortho_a_to_c.y;
	ortho_a_to_c.y = -temp;
	SliderVector2 dot = {
		.x = b.x - a.x,
		.y = b.y - a.y
	};
	if ((ortho_a_to_c.x * dot.x + ortho_a_to_c.y + dot.y) < 0) {
		direction = -direction;
		theta_range = 2 * M_PI - theta_range;
	}
	// end
	const float circular_arc_tolerance = 0.1f;
	int amount_points = 2 * radius <= circular_arc_tolerance ? 2 : (int) fmax(2, (int) ceil(theta_range / (2 * acos(1 - circular_arc_tolerance / radius))));
	LinkedList *output = NULL;
	for (int i = 0; i < amount_points; i++) {
		double fract = (double) i / (amount_points - 1);
		double theta = theta_start + direction * fract * theta_range;
		SliderVector2 *o = calloc(1, sizeof(*o));
		o->x = ((float) cos(theta) * radius) + centre.x;
		o->y = ((float) sin(theta) * radius) + centre.y;
		ou_linkedlist_add(&output, o, 0);
	}
	while (output != NULL) {
		SliderVector2 *temp_data = NULL;
		unsigned int temp_len = 0;
		ou_linkedlist_remove(&output, (void *)&temp_data, &temp_len);
		*result = realloc(*result, ++(*len_result) * sizeof(**result));
		*(*result + *len_result - 1) = *temp_data;
	}
}

void oos_slider_beziersubdivide(SliderVector2 *control_points, SliderVector2 *l, SliderVector2 *r, SliderVector2 *subdivision_buffer, int count) {
	SliderVector2 *midpoints = subdivision_buffer;
	for (int i = 0; i < count; i++) {
		(midpoints + i)->x = (control_points + i)->x;
		(midpoints + i)->y = (control_points + i)->y;
	}
	for(int i = 0; i < count; i++) {
		(l + i)->x = (midpoints + i)->x;
		(l + i)->y = (midpoints + i)->y;
		(r + count - i - 1)->x = (midpoints + count - i - 1)->x;
		(r + count - i - 1)->y = (midpoints + count - i - 1)->y;
		for (int j = 0; j < count - i - 1; j++) {
			(midpoints + j)->x = ((midpoints + j)->x + (midpoints + j + 1)->x) / 2;
			(midpoints + j)->y = ((midpoints + j)->y + (midpoints + j + 1)->y) / 2;
		}
	}
}

void oos_slider_approximatebezier(SliderVector2 **result, unsigned int *len_result, SliderVector2 *vertices, unsigned int len) {
	int p = 0;
	LinkedList *output = NULL;
	int n = len - 1;
	if (n < 0) {
		return;
	}
	Stack *to_flatten = NULL;
	Stack *free_buffers = NULL;
	if (p > 0 && p < n) {
		for (int i = 0; i < n - p; i++) {
			SliderVector2 *sub_bezier = calloc(p + 1, sizeof(*sub_bezier));
			*(sub_bezier + 0) = *(vertices + i);
			for (int j = 0; j < p - 1; j++) {
				*(sub_bezier + j + 1) = *(vertices + i + 1);
				for (int k = 1; k < 0 - j; k++) {
					int l = fminl(k, n - p - 1);
					(vertices + i + k)->x = (l * (vertices + i + k)->x + (vertices + i + k + 1)->x / (l + 1));
					(vertices + i + k)->y = (l * (vertices + i + k)->y + (vertices + i + k + 1)->y / (l + 1));
				}
			}
			*(sub_bezier + p) = *(vertices + i + 1);
			ou_stack_push(&to_flatten, sub_bezier, p + 1);
		}
		unsigned int other_points_len = len - (n - p);
		SliderVector2 *other_points = calloc(other_points_len, sizeof(*other_points));
		for (int i = (n - p); i < len; i++) {
			*(other_points + i - (n - p)) = *(vertices + i);
		}
		ou_stack_push(&to_flatten, other_points, other_points_len);
		ou_stack_reverse(&to_flatten);
	} else {
		p = n;
		ou_stack_push(&to_flatten, vertices, len);
	}
	SliderVector2 *subdivision_buffer1 = calloc(p + 1, sizeof(*subdivision_buffer1));
	SliderVector2 *subdivision_buffer2 = calloc(p * 2 + 1, sizeof(*subdivision_buffer2));
	SliderVector2 *left_child = subdivision_buffer2;
	while (to_flatten != NULL && to_flatten->data != NULL) {
		SliderVector2 *parent = NULL;
		unsigned int parent_len = 0;
		ou_stack_pop(&to_flatten, (void *)&parent, &parent_len);
		bool bezier_is_flat_enough = true;
		for (int i = 1; i < parent_len - 1; i++) {
			float x = (vertices + i - 1)->x - 2 * (vertices + i)->x + (vertices + i + 1)->x;
			float y = (vertices + i - 1)->y - 2 * (vertices + i)->y + (vertices + i + 1)->y;
			float length = sqrtf(x * x + y * y);
			const float bezier_tolerance = 0.25f;
			if (length * length > bezier_tolerance * bezier_tolerance * 4) {
				bezier_is_flat_enough = true;
				break;
			}
		}
		if (bezier_is_flat_enough) {
			// bezierApproximate
			oos_slider_beziersubdivide(parent, subdivision_buffer2, subdivision_buffer1, subdivision_buffer1, p + 1);
			for (int i = 0; i < p; i++) {
				*(subdivision_buffer2 + p + 1 + i) = *(subdivision_buffer1 + i + 1);
			}
			ou_linkedlist_add(&output, (parent + 0), 0);
			for (int i = 1; i < p; i++) {
				int index = 2 * i;
				SliderVector2 *p = calloc(1, sizeof(*p));
				p->x = 0.25f * ((subdivision_buffer2 + index - 1)->x + 2 * (subdivision_buffer2 + index)->x + (subdivision_buffer2 + index + 1)->x);
				p->y = 0.25f * ((subdivision_buffer2 + index - 1)->y + 2 * (subdivision_buffer2 + index)->y + (subdivision_buffer2 + index + 1)->y);
				ou_linkedlist_add(&output, p, 0);
			}
			// end
			ou_stack_push(&free_buffers, parent, parent_len);
			continue;
		}
		unsigned int free_buffers_count = 0;
		ou_stack_count(free_buffers, &free_buffers_count);
		SliderVector2 *right_child = NULL;
		unsigned int right_child_len = 0;
		if (free_buffers_count > 0) {
			ou_stack_pop(&free_buffers, (void *)&right_child, &right_child_len);
		} else {
			right_child = calloc(p + 1, sizeof(*right_child));
		}
		oos_slider_beziersubdivide(parent, left_child, right_child, subdivision_buffer1, p + 1);
		for (int i = 0; i < p + 1; i++) {
			*(parent + i) = *(left_child + i);
		}
		ou_stack_push(&to_flatten, &right_child, right_child_len);
		ou_stack_push(&to_flatten, &parent, parent_len);
	}
	ou_linkedlist_add(&output, (vertices + n), 0);
	while (output != NULL) {
		SliderVector2 *temp_data = NULL;
		unsigned int temp_len = 0;
		ou_linkedlist_remove(&output, (void *)&temp_data, &temp_len);
		*result = realloc(*result, ++(*len_result) * sizeof(**result));
		*(*result + *len_result - 1) = *temp_data;
	}

	free(to_flatten);
	free(free_buffers);
	free(subdivision_buffer1);
	free(subdivision_buffer2);
}

void oos_slider_calculatesubpath(SliderVector2 **result, unsigned int *len_result, SliderVector2 *vertices, unsigned int len, SliderType type) {
	switch (type) {
		case slidertype_linear:
			oos_slider_linearapproximate(result, len_result, vertices, len);
			return;
		
		case slidertype_catmull:
			oos_slider_catmullapproximate(result, len_result, vertices, len);
			return;

		case slidertype_bezier:
			break;

		case slidertype_perfectcurve:
			if (len != 3) {
				break;
			}
			oos_slider_approximatecirculararc(result, len_result, vertices, len);
			if (*len_result == 0) {
				break;
			}
			return;
	}
	oos_slider_approximatebezier(result, len_result, vertices, len);
}

void oos_slider_positionat(SliderVector2 *vector, double progress, Slider *slider) {
	if (vector == NULL || slider == NULL) {
		return;
	}
	// calculatePath()
	if (slider->calculate_path == NULL && slider->calculatepath_len == 0) {
		slider->control_point = calloc(1, sizeof(*slider->control_point));
		(slider->control_point + 0)->type = &slider->ho_data->curve_type;
		(slider->control_point + 0)->x = 0;
		(slider->control_point + 0)->y = 0;
		slider->controlpoint_len++;
		for (int i = 0; i < slider->ho_data->num_curve; i++) {
			if ((slider->control_point + slider->controlpoint_len - 1)->x != (slider->ho_data->curves + i)->x - slider->start_position.x
				|| (slider->control_point + slider->controlpoint_len - 1)->y != (slider->ho_data->curves + i)->y - slider->start_position.y) {
				slider->control_point = realloc(slider->control_point, (slider->controlpoint_len + 1) * sizeof(*slider->control_point));
				(slider->control_point + slider->controlpoint_len)->type = slider->ho_data->num_curve > i + 1 ? &slider->ho_data->curve_type : NULL; // TODO look into this with all different curve type
				(slider->control_point + slider->controlpoint_len)->x = (slider->ho_data->curves + i)->x - slider->start_position.x;
				(slider->control_point + slider->controlpoint_len)->y = (slider->ho_data->curves + i)->y - slider->start_position.y;
				slider->controlpoint_len++;
			}
		}
		int start = 0;
		for (int i = 0; i < slider->controlpoint_len; i++) {
			if ((slider->control_point + i)->type == NULL && i < slider->controlpoint_len - 1) {
				continue;
			}
			unsigned int segment_len = i - start + 1;
			SliderVector2 segment_vertices[segment_len];
			for (int j = 0, k = start; k < segment_len + start; j++, k++) {
				segment_vertices[j].x = (slider->control_point + k)->x;
				segment_vertices[j].y = (slider->control_point + k)->y;
			}
			SliderType segment_type = (slider->control_point + start)->type != NULL ? *(slider->control_point + start)->type : slidertype_linear;
			
			SliderVector2 *result = NULL;
			unsigned int len_result = 0;
			oos_slider_calculatesubpath(&result, &len_result, segment_vertices, segment_len, segment_type);
			for (int j = 0; j < len_result; j++) {
				if (slider->calculatepath_len == 0
					|| !((slider->calculate_path + slider->calculatepath_len - 1)->x == (result + j)->x
						&& (slider->calculate_path + slider->calculatepath_len - 1)->y == (result + j)->y)) {
					slider->calculate_path = realloc(slider->calculate_path, (slider->calculatepath_len + 1) * sizeof(*slider->calculate_path));
					(slider->calculate_path + slider->calculatepath_len)->x = (result + j)->x;
					(slider->calculate_path + slider->calculatepath_len)->y = (result + j)->y;
					slider->calculatepath_len++;
				}
			}
			free(result);
			start = i;
		}
	}
	
	// caluclateLength()
	if (slider->cumulative_length == NULL && slider->cumulativelength_len == 0) {
		slider->cumulative_length = calloc(1, sizeof(*slider->cumulative_length));
		double calculated_length = 0;
		*(slider->cumulative_length + slider->cumulativelength_len) = 0;
		slider->cumulativelength_len++;
		for (int i = 0; i < slider->calculatepath_len - 1; i++) {
			SliderVector2 diff = {
				.x = (slider->calculate_path + i + 1)->x - (slider->calculate_path + i)->x,
				.y = (slider->calculate_path + i + 1)->y - (slider->calculate_path + i)->y
			};
			calculated_length += sqrt((diff.x * diff.x) + (diff.y * diff.y));
			slider->cumulative_length = realloc(slider->cumulative_length, (slider->cumulativelength_len + 1) * sizeof(*slider->cumulative_length));
			*(slider->cumulative_length + slider->cumulativelength_len) = calculated_length;
			slider->cumulativelength_len++;
		}
		if (calculated_length != slider->path.distance) { // Should I be worried with `ExpectedDistance.Value is double`? It seems that it is nullable
			if (slider->controlpoint_len >= 2
				&& (((slider->control_point + slider->controlpoint_len - 1)->x == (slider->control_point + slider->controlpoint_len - 2)->x
					&& (slider->control_point + slider->controlpoint_len - 1)->y == (slider->control_point + slider->controlpoint_len - 2)->y))
				&& slider->path.distance > calculated_length) {
				slider->cumulative_length = realloc(slider->cumulative_length, (slider->cumulativelength_len + 1) * sizeof(*slider->cumulative_length));
				*(slider->cumulative_length + slider->cumulativelength_len) = calculated_length;
				slider->cumulativelength_len++;
				goto caluclatelength_goto;
			}
				slider->cumulative_length = realloc(slider->cumulative_length, (slider->cumulativelength_len - 1) * sizeof(*slider->cumulative_length));
				slider->cumulativelength_len--;
			int path_end_index = slider->calculatepath_len - 1;
			if (calculated_length > slider->path.distance) {
				while (slider->cumulativelength_len > 0 && *(slider->cumulative_length + slider->cumulativelength_len - 1) >= slider->path.distance) {
					slider->cumulative_length = realloc(slider->cumulative_length, (slider->cumulativelength_len - 1) * sizeof(*slider->cumulative_length));
					slider->cumulativelength_len--;
					for (int i = path_end_index; i < slider->calculatepath_len; i++) {
						// TODO an error keep coming up here; test through the entire code to make sure this is working correctly
						*(slider->cumulative_length + i) = *(slider->cumulative_length + i + 1);
					}
					slider->calculatepath_len--;
				}
			}
			if (path_end_index <= 0) {
				slider->cumulative_length = realloc(slider->cumulative_length, (slider->cumulativelength_len + 1) * sizeof(*slider->cumulative_length));
				*(slider->cumulative_length + slider->cumulativelength_len) = 0;
				slider->cumulativelength_len++;
				goto caluclatelength_goto;
			}
			SliderVector2 dir = {
				.x = (slider->calculate_path + path_end_index)->x - (slider->calculate_path + path_end_index - 1)->x,
				.y = (slider->calculate_path + path_end_index)->y - (slider->calculate_path + path_end_index - 1)->y
			};
			float distance = sqrtf((dir.x * dir.x) + (dir.y * dir.y));
			dir.x = dir.x / distance;
			dir.y = dir.y / distance;
			(slider->calculate_path + path_end_index)->x = (slider->calculate_path + path_end_index - 1)->x + dir.x * (float) (slider->path.distance - *(slider->cumulative_length + slider->cumulativelength_len - 1));
			(slider->calculate_path + path_end_index)->y = (slider->calculate_path + path_end_index - 1)->y + dir.y * (float) (slider->path.distance - *(slider->cumulative_length + slider->cumulativelength_len - 1));
			slider->cumulative_length = realloc(slider->cumulative_length, (slider->cumulativelength_len + 1) * sizeof(*slider->cumulative_length));
			*(slider->cumulative_length + slider->cumulativelength_len) = slider->path.distance;
			slider->cumulativelength_len++;
		}

		caluclatelength_goto:;
	}

	double d = fmax(0, fmin(progress, 1)) * slider->path.distance;
	
	// indexOfDisatnce(d)
	int i = 0;
	int l = 0;
	int r = slider->cumulativelength_len - 1;
	int pivot;
	while (l <= r) {
		pivot = (r + l) / 2;
		if (*(slider->cumulative_length + pivot) < d) {
			l = pivot + 1;
		} else if (*(slider->cumulative_length + pivot) > d) {
			r = pivot - 1;
		} else {
			i = *(slider->cumulative_length + pivot);
			break;
		}
	}
	if (i == 0) {
		i = l;
	}

	// interpolateVertices(indexOfDisatnce(d), d)
	if (slider->calculatepath_len == 0) {
		vector->x = 0;
		vector->y = 0;
		goto interpolatevertices_goto;
	}
	if (i <= 0) {
		vector->x = (slider->calculate_path + 0)->x;
		vector->y = (slider->calculate_path + 0)->y;
		goto interpolatevertices_goto;
	}
	if (i >= slider->calculatepath_len) {
		vector->x = (slider->calculate_path + slider->calculatepath_len - 1)->x;
		vector->y = (slider->calculate_path + slider->calculatepath_len - 1)->y;
		goto interpolatevertices_goto;
	}
	SliderVector2 p0 = {
		.x = (slider->calculate_path + i - 1)->x,
		.y = (slider->calculate_path + i - 1)->y
	};
	SliderVector2 p1 = {
		.x = (slider->calculate_path + i)->x,
		.y = (slider->calculate_path + i)->y
	};
	double d0 = *(slider->cumulative_length + i - 1);
	double d1 = *(slider->cumulative_length + i);
	if (fabs(d0 - d1) <= 1e-7) {
		vector->x = p0.x;
		vector->y = p0.y;
		goto interpolatevertices_goto;
	}
	double w = (d - d0) / (d1 - d0);
	vector->x = p0.x + (p1.x - p0.x) * (float) w;
	vector->y = p0.y + (p1.y - p0.y) * (float) w;

	interpolatevertices_goto:;
}

void oos_slider_createnestedhitobjects(Slider *slider) {
	SliderEventDescriptor *slider_event;
	while ((slider_event = oos_slider_generate(slider->start_time, slider->span_duration, slider->velocity, slider->tick_distance, slider->path.distance, slider->span_count, slider->legacy_last_tick_offset)) != NULL) {
		switch (slider_event->type) {
			case sliderevent_tick: {
				slider->nested = realloc(slider->nested, (slider->num_nested + 1) * sizeof(*slider->nested));
				// (slider->nested + slider->num_nested)->type = tick;
				(slider->nested + slider->num_nested)->time = slider_event->time;
				SliderVector2 position;
				oos_slider_positionat(&position, slider_event->path_progress, slider);
				(slider->nested + slider->num_nested)->x = slider->start_position.x + position.x;
				(slider->nested + slider->num_nested)->y = slider->start_position.y + position.y;
				slider->num_nested++;
				break;
			}

			case sliderevent_head: {
				slider->nested = realloc(slider->nested, (slider->num_nested + 1) * sizeof(*slider->nested));
				// (slider->nested + slider->num_nested)->type = head;
				(slider->nested + slider->num_nested)->time = slider_event->time;
				(slider->nested + slider->num_nested)->x = slider->start_position.x;
				(slider->nested + slider->num_nested)->y = slider->start_position.y;
				slider->num_nested++;
				break;
			}

			case sliderevent_repeat: {
				slider->nested = realloc(slider->nested, (slider->num_nested + 1) * sizeof(*slider->nested));
				// (slider->nested + slider->num_nested)->type = repeat;
				(slider->nested + slider->num_nested)->time = slider_event->time;
				SliderVector2 position;
				oos_slider_positionat(&position, slider_event->path_progress, slider);
				(slider->nested + slider->num_nested)->x = slider->start_position.x + position.x;
				(slider->nested + slider->num_nested)->y = slider->start_position.y + position.y;
				slider->num_nested++;
				break;
			}

			// This seems to cause more trouble than it does help so I'm commenting this out for the time being
			case sliderevent_legacylasttick:
				break;
			// case sliderevent_legacylasttick: {
			//     slider->nested = realloc(slider->nested, (slider->num_nested + 1) * sizeof(*slider->nested));
			//     // (slider->nested + slider->num_nested)->type = tail;
			//     (slider->nested + slider->num_nested)->time = slider_event->time;
			//     double progress = 1 * (int) slider->span_count % 1;
			//     if ((int) (1 * slider->span_count) % 2 == 1) {
			//         progress = 1 - progress;
			//     }
			//     SliderVector2 position;
			//     oos_slider_positionat(&position, progress, slider);
			//     (slider->nested + slider->num_nested)->x = slider->start_position.x + position.x;
			//     (slider->nested + slider->num_nested)->y = slider->start_position.y + position.y;
			//     slider->num_nested++;
			//     break;
			// }

			// Source code just ignores this but idc
			case sliderevent_tail: {
				slider->nested = realloc(slider->nested, (slider->num_nested + 1) * sizeof(*slider->nested));
				// (slider->nested + slider->num_nested)->type = tail;
				(slider->nested + slider->num_nested)->time = slider_event->time;
				SliderVector2 position;
				oos_slider_positionat(&position, slider_event->path_progress, slider);
				(slider->nested + slider->num_nested)->x = slider->start_position.x + position.x;
				(slider->nested + slider->num_nested)->y = slider->start_position.y + position.y;
				slider->num_nested++;
				break;
			}
		}
	}
}

SliderEventDescriptor *oos_slider_generate(double start_time, double span_duration, double velocity, double tick_distance, double total_distance, int span_count, double *legacy_last_tick_offset) {
	static int state = 0;
	static SliderEventDescriptor *object = NULL;
	if (object != NULL) {
		free(object);
		object = NULL;
	}
	switch (state) {
		case 0:;
			const double max_length = 100000;
			static double length;
			length = fmin(max_length, total_distance);
			static double ftick_distance;
			ftick_distance = fmin(tick_distance, fmax(0, length));
			static double min_distance_from_end;
			min_distance_from_end = velocity * 10;

			{
				state = 1;
				object = calloc(1, sizeof(*object));
				object->type = sliderevent_head;
				object->span_index = 0;
				object->span_start_time = start_time;
				object->time = start_time;
				object->path_progress = 0;
				return object;
			}
		
		case 1:
			if (ftick_distance != 0) {
				static int span;
				for (span = 0; span < span_count; span++) {
					static double span_start_time;
					span_start_time = start_time + span * span_duration;
					static bool reversed;
					reversed = span % 2 == 1;
					
					{
						state = 2;
						while ((object = oos_slider_generateticks(span, span_start_time, span_duration, reversed, length, ftick_distance, min_distance_from_end)) != NULL) {
							return object;
		case 2:;
						}
					}

					state = 3;
					if (span < span_count - 1) {
						object = calloc(1, sizeof(*object));
						object->type = sliderevent_repeat;
						object->span_index = span;
						object->span_start_time = start_time + span * span_duration;
						object->time = span_start_time + span_duration;
						object->path_progress = (span + 1) % 2;
						return object;
					}
		case 3:;
				}
			}

			static double total_duration;
			total_duration = span_count * span_duration;
			static int final_span_index;
			final_span_index = span_count - 1;
			double final_span_start_time = start_time + final_span_index * span_duration;
			double final_span_end_time = fmax(start_time + total_duration / 2, (final_span_start_time + span_duration) - (legacy_last_tick_offset != NULL ? *legacy_last_tick_offset : 0));
			static double final_progress;
			final_progress = (final_span_end_time - final_span_start_time) / span_duration;
			if (span_count % 2 == 0) {
				final_progress = 1 - final_progress;
			}
			{
				state = 4;
				object = calloc(1, sizeof(*object));
				object->type = sliderevent_legacylasttick;
				object->span_index = final_span_index;
				object->span_start_time = final_span_start_time;
				object->time = final_span_end_time;
				object->path_progress = final_progress;
				return object;
			}
			
		case 4:
			{
				state = 5;
				object = calloc(1, sizeof(*object));
				// Ssource code seems to break so I'm just copying pasting parts of legacylasttick
				object->type = sliderevent_tail;
				object->span_index = final_span_index;
				object->span_start_time = start_time + (span_count - 1) * span_duration;
				object->time = start_time + total_duration;
				object->path_progress = (int) span_duration;
				return object;
			}

		case 5:;
	}
	
	state = 0;
	return NULL;
}

SliderEventDescriptor *oos_slider_generateticks(int span_index, double span_start_time, double span_duration, bool reversed, double length, double tick_distance, double min_distance_from_end) {
	static int state = 0;
	static SliderEventDescriptor *object = NULL;
	switch (state) {
		case 0:;
			state = 1;
			static double d;
			for (d = tick_distance; d <= length; d += tick_distance) {
				if (d >= length - min_distance_from_end) {
					break;
				}
				double path_progress = d / length;
				double time_progress = reversed ? 1 - path_progress : path_progress;

				object = calloc(1, sizeof(*object));
				object->type = sliderevent_tick;
				object->span_index = span_index;
				object->span_start_time = span_start_time;
				object->time = span_start_time + time_progress * span_duration;
				object->path_progress = path_progress;
				return object;
		case 1:;
			}
	}
	state = 0;
	return NULL;
}