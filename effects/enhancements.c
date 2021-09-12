#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "composite.h"
#include "palettes/palettes.h"
#include "enhancements.h"

static void clut1d(int offset, int width, int height, int rowstride, uint8_t *src, uint8_t *dst, const uint8_t *table);
static void clut2d(int offset_x, int offset_y, int width, int height, int rowstride, uint8_t *src, uint8_t *dst, const uint8_t *table);

void
enhance_none(Image *dst, Image *src)
{
	memcpy(dst->pixbuf, src->pixbuf, src->height*src->rowstride);
}

void
enhance_122(Image *dst, Image *src)
{
	int i, j;

	uint8_t *src_pixbuf, *dst_pixbuf;

	src_pixbuf = src->pixbuf;
	dst_pixbuf = dst->pixbuf;

	for (i=0; i<dst->height; i++) {
		for (j=0; j<3*dst->width; j+=3) {
			/* Red <- green */
			dst_pixbuf[j+0] = src_pixbuf[j+1];
			/* Green <- green */
			dst_pixbuf[j+1] = src_pixbuf[j+1];
			/* Blue <- blue */
			dst_pixbuf[j+2] = src_pixbuf[j+2];
		}

		src_pixbuf += src->rowstride;
		dst_pixbuf += dst->rowstride;
	}
}

void
enhance_211(Image *dst, Image *src)
{
	int i, j;

	uint8_t *src_pixbuf, *dst_pixbuf;

	src_pixbuf = src->pixbuf;
	dst_pixbuf = dst->pixbuf;

	for (i=0; i<dst->height; i++) {
		for (j=0; j<3*dst->width; j+=3) {
			/* Red <- green */
			dst_pixbuf[j+0] = src_pixbuf[j+1];
			/* Green <- blue */
			dst_pixbuf[j+1] = src_pixbuf[j+2];
			/* Blue <- blue*/
			dst_pixbuf[j+2] = src_pixbuf[j+2];

		}

		src_pixbuf += src->rowstride;
		dst_pixbuf += dst->rowstride;
	}
}

void
enhance_vegetation(Image *dst, Image *src)
{
	clut2d(2, 1, dst->width, dst->height, dst->rowstride, src->pixbuf, dst->pixbuf, vegetation_clut.pixel_data);
}

void
enhance_hvc(Image *dst, Image *src)
{
	clut2d(0, 1, dst->width, dst->height, dst->rowstride, src->pixbuf, dst->pixbuf, hvc_clut.pixel_data);
}

void
enhance_hvc_precip(Image *dst, Image *src)
{
	clut2d(0, 1, dst->width, dst->height, dst->rowstride, src->pixbuf, dst->pixbuf, hvc_precip_clut.pixel_data);
}

void
enhance_thermal(Image *dst, Image *src)
{
	clut1d(0, dst->width, dst->height, dst->rowstride, src->pixbuf, dst->pixbuf, thermal_clut.pixel_data);
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
