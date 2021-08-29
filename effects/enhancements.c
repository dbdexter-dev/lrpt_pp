#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "composite.h"
#include "enhancements.h"

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
	printf("Unimplemented\n");
}

void
enhancement_thermal(int width, int height, uint8_t *src, uint8_t *dst)
{
	printf("Unimplemented\n");
}

void
enhancement_hvc(int width, int height, uint8_t *src, uint8_t *dst)
{
	printf("Unimplemented\n");
}

