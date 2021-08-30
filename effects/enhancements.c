#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "composite.h"
#include "palettes/palettes.h"
#include "enhancements.h"

static void clut1d(int offset, int width, int height, int rowstride, uint8_t *src, uint8_t *dst, const uint8_t *table);
static void clut2d(int offset_x, int offset_y, int width, int height, int rowstride, uint8_t *src, uint8_t *dst, const uint8_t *table);

void
enhancement_none(int width, int height, int rowstride, uint8_t *src, uint8_t *dst)
{
	memcpy(dst, src, height*rowstride);
}

void
enhancement_122(int width, int height, int rowstride, uint8_t *src, uint8_t *dst)
{
	int i, j;

	for (i=0; i<height; i++) {
		for (j=0; j<3*width; j+=3) {
			/* Red <- green */
			dst[j+0] = src[j+1];
			/* Green <- green */
			dst[j+1] = src[j+1];
			/* Blue <- blue */
			dst[j+2] = src[j+2];
		}

		src += rowstride;
		dst += rowstride;
	}
}

void
enhancement_211(int width, int height, int rowstride, uint8_t *src, uint8_t *dst)
{
	int i, j;

	for (i=0; i<height; i++) {
		for (j=0; j<3*width; j+=3) {
			/* Red <- green */
			dst[j+0] = src[j+1];
			/* Green <- blue */
			dst[j+1] = src[j+2];
			/* Blue <- blue*/
			dst[j+2] = src[j+2];

		}

		src += rowstride;
		dst += rowstride;
	}
}

void
enhancement_vegetation(int width, int height, int rowstride, uint8_t *src, uint8_t *dst)
{
	clut2d(2, 1, width, height, rowstride, src, dst, vegetation_clut.pixel_data);
}

void
enhancement_hvc(int width, int height, int rowstride, uint8_t *src, uint8_t *dst)
{
	clut2d(0, 1, width, height, rowstride, src, dst, hvc_clut.pixel_data);
}

void
enhancement_hvc_precip(int width, int height, int rowstride, uint8_t *src, uint8_t *dst)
{
	clut2d(0, 1, width, height, rowstride, src, dst, hvc_precip_clut.pixel_data);
}

void
enhancement_thermal(int width, int height, int rowstride, uint8_t *src, uint8_t *dst)
{
	clut1d(0, width, height, rowstride, src, dst, thermal_clut.pixel_data);
}


static void
clut2d(int offset_x, int offset_y, int width, int height, int rowstride, uint8_t *src, uint8_t *dst, const uint8_t *table)
{
	int i, j;
	uint8_t x, y;

	for (i=0; i<height; i++) {
		for (j=0; j<3*width; j+=3) {
			x = src[j+offset_x];
			y = src[j+offset_y];

			memcpy(dst+j, &table[3*(x*256+y)], 3);
		}
		src += rowstride;
		dst += rowstride;
	}
}

static void
clut1d(int offset, int width, int height, int rowstride, uint8_t *src, uint8_t *dst, const uint8_t *table)
{
	int i, j;
	uint8_t x;

	for (i=0; i<height; i++) {
		for (j=0; j<3*width; j+=3) {
			x = src[j+offset];
			memcpy(dst+j, &table[3*x], 3);
		}
		src += rowstride;
		dst += rowstride;
	}
}

char*
enhancement_str(Enhancement e)
{
	switch (e) {
		case NONE:
			return "default";
		case VISIBLE:
			return "122";
		case VEGETATION:
			return "vegetation";
		case VEG_IR:
			return "211";
		case HVC:
			return "hvc";
		case HVC_PRECIP:
			return "hvc_precip";
		case THERMAL:
			return "thermal";
	}
}
