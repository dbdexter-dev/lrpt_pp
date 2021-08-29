#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "composite.h"
#include "palettes/palettes.h"
#include "enhancements.h"

static void clut(int offset_x, int offset_y, int width, int height, uint8_t *src, uint8_t *dst, const uint8_t *table);

void
enhancement_none(int width, int height, uint8_t *src, uint8_t *dst)
{
	memcpy(dst, src, width*height*3);
}

void
enhancement_122(int width, int height, uint8_t *src, uint8_t *dst)
{
	int i, j;

	for (i=0; i<width; i++) {
		for (j=0; j<height; j++) {
			src++;

			/* Red <- green */
			*dst++ = *src;
			/* Green <- green */
			*dst++ = *src++;
			/* Blue <- blue */
			*dst++ = *src++;
		}
	}
}

void
enhancement_211(int width, int height, uint8_t *src, uint8_t *dst)
{
	int i, j;

	for (i=0; i<width; i++) {
		for (j=0; j<height; j++) {
			src++;
			/* Red <- green */
			*dst++ = *src++;
			/* Green <- blue */
			*dst++ = *src;
			/* Blue <- blue*/
			*dst++ = *src++;
		}
	}
}

void
enhancement_vegetation(int width, int height, uint8_t *src, uint8_t *dst)
{
	clut(2, 1, width, height, src, dst, vegetation_clut.pixel_data);
}

void
enhancement_hvc(int width, int height, uint8_t *src, uint8_t *dst)
{
	clut(0, 1, width, height, src, dst, hvc_clut.pixel_data);
}

void
enhancement_hvc_precip(int width, int height, uint8_t *src, uint8_t *dst)
{
	clut(0, 1, width, height, src, dst, hvc_precip_clut.pixel_data);
}

void
enhancement_thermal(int width, int height, uint8_t *src, uint8_t *dst)
{
	printf("Unimplemented\n");
}


static void
clut(int offset_x, int offset_y, int width, int height, uint8_t *src, uint8_t *dst, const uint8_t *table)
{
	int i, j;
	uint8_t x, y;

	for (i=0; i<width; i++) {
		for (j=0; j<height; j++) {
			x = src[offset_x];
			y = src[offset_y];

			src += 3;

			memcpy(dst, &table[3*(x*256+y)], 3);
			dst += 3;
		}
	}
}
