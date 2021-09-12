#include <math.h>
#include <stdlib.h>
#include "rectify.h"
#include "utils.h"

#ifndef M_PI
#define M_PI 3.1415926536
#endif

static const float earth_radius = 6371.0;

static float earth2sat(float angle, float altitude);
static float sat2earth(float angle, float altitude);
static float cfactor(float sat_angle, float altitude);

void
rectify(Image *dst, Image *src, float altitude, float scan_angle)
{
	const float scan_delta = scan_angle / src->width;

	uint8_t *src_px, *dst_px;
	int i, row, col, dst_width, cur_idx, prev_idx;
	float angle, sum, tmp, percent;
	float *cfactors, *mapping;

	/* Compute the correction factor for each pixel */
	cfactors = malloc(src->width * sizeof(*cfactors));
	for (i=0; i<src->width; i++) {
		angle = -scan_angle/2 + i*scan_delta;
		cfactors[i] = cfactor(angle, altitude);
	}

	/* Compute the input -> output mapping */
	sum = 0;
	for (i=0; i<src->width; i++) {
		tmp = cfactors[i];
		cfactors[i] = sum;
		sum += tmp;
	}

	/* Store the destination image width */
	dst_width = ceil(cfactors[src->width-1]);
	mapping = malloc(dst_width * sizeof(*mapping));

	/* Compute the output -> input mapping */
	cur_idx = 0;
	for (i=0; i<dst_width; i++) {
		while (cfactors[cur_idx] < i) cur_idx++;

		if (cur_idx == 0) {
			mapping[i] = 0;
		} else {
			tmp = (i - cfactors[cur_idx-1]) / (cfactors[cur_idx] - cfactors[cur_idx-1]);
			mapping[i] = cur_idx - 1 + tmp;
		}
	}

	/* Build an image with the expected width */
	if (dst->pixbuf) image_deinit(dst);
	image_init(dst, dst_width, src->height, dst_width*3, 24, NULL);

	/* Stretch the image */
	src_px = src->pixbuf;
	dst_px = dst->pixbuf;

	for (row=0; row<dst->height; row++) {
		for (col=0; col<dst->width; col++) {
			prev_idx = floor(mapping[col]);
			cur_idx = ceil(mapping[col]);

			percent = mapping[col] - floor(mapping[col]);

			dst_px[3*col] = src_px[3*prev_idx] * (1 - percent)
			              + src_px[3*cur_idx] * percent;
			dst_px[3*col + 1] = src_px[3*prev_idx + 1] * (1 - percent)
			                  + src_px[3*cur_idx + 1] * percent;
			dst_px[3*col + 2] = src_px[3*prev_idx + 2] * (1 - percent)
			                  + src_px[3*cur_idx + 2] * percent;
		}

		src_px += src->rowstride;
		dst_px += dst->rowstride;
	}

	free(cfactors);
	free(mapping);
}

void
rectify_m2(Image *dst, Image *src)
{
	rectify(dst, src, M2_ALTITUDE, M2_SCAN_ANGLE);
}

static float
earth2sat(float angle, float altitude)
{
	const float b = earth_radius * sin(angle);
	const float h = altitude + earth_radius * (1 - cos(angle));

	return atan(b/h);
}

static float
sat2earth(float angle, float altitude)
{
	const float b = sin(angle) * (earth_radius + altitude);
	const float gamma = acos(b / earth_radius);

	return M_PI/2 - gamma - angle;
}

static float
cfactor(float sat_angle, float altitude)
{
	const float earth_angle = sat2earth(sat_angle, altitude);
	const float dh = earth_radius * (1 - cos(earth_radius));
	const float cfactor = (1 + dh/altitude) * 1/(cos(sat_angle) * cos(sat_angle + earth_angle));

	return cfactor;
}
