#include <stdlib.h>
#include "image.h"

void
image_init(Image *ch, int width, int height, int rowstride, uint8_t *pixbuf)
{
	if (!ch) return;

	ch->width = width;
	ch->height = height;
	ch->rowstride = rowstride;

	/* Initialize mutex */
	pthread_mutex_init(&ch->mutex, NULL);

	/* Initialize pixbuf */
	if (pixbuf) {
		ch->pixbuf = pixbuf;
		ch->owned = 0;
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
