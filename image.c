#include <stdlib.h>
#include <string.h>
#include "image.h"

void
image_init(Image *ch, int width, int height, int rowstride, int bpp, uint8_t *pixbuf)
{
	int row, col;
	uint8_t *ptr;

	if (!ch) return;

	ch->width = width;
	ch->height = height;
	ch->rowstride = rowstride;

	/* Initialize mutex */
	pthread_mutex_init(&ch->mutex, NULL);

	/* Initialize pixbuf */
	if (pixbuf) {
		switch (bpp) {
			case 24:
				ch->pixbuf = pixbuf;
				ch->owned = 0;
				break;
			case 32:
				/* Remove alpha channel */
				ch->rowstride = ch->rowstride * 3 / 4;
				ch->pixbuf = malloc(height*ch->rowstride);
				ch->owned = 1;

				ptr = ch->pixbuf;
				for (row=0; row<height; row++) {
					for (col=0; col<width; col++) {
						memcpy(ptr+3*col, pixbuf+4*col, 3);
					}
					ptr += ch->rowstride;
					pixbuf += rowstride;
				}
				break;
			default:
				ch->pixbuf = pixbuf;
				ch->owned = 0;
				break;
		}
	} else {
		ch->pixbuf = malloc(height*rowstride);
		ch->owned = 1;
	}
}

void
image_deinit(Image *ch)
{
	if (ch->owned) free(ch->pixbuf);
	ch->pixbuf = NULL;
	ch->owned = 0;
}

void
image_resize_match(Image *ch, const Image *orig)
{
	if (ch->width == orig->width && ch->height == orig->height && ch->rowstride == orig->rowstride) return;

	if (ch->pixbuf) image_deinit(ch);

	image_init(ch, orig->width, orig->height, orig->rowstride, 24, NULL);
}


uint8_t*
image_get_pixels(Image *ch)
{
	pthread_mutex_lock(&ch->mutex);
	return ch->pixbuf;
}

void
image_release_pixels(Image *ch)
{
	pthread_mutex_unlock(&ch->mutex);
}
