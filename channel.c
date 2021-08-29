#include <stdlib.h>
#include "channel.h"

void
channel_init(Channel *ch, int width, int height, uint8_t *pixbuf)
{
	if (!ch) return;

	ch->width = width;
	ch->height = height;

	/* Initialize mutex */
	pthread_mutex_init(&ch->mutex, NULL);

	/* Initialize pixbuf */
	if (pixbuf) {
		ch->pixbuf = pixbuf;
		ch->owned = 0;
	} else {
		ch->pixbuf = malloc(width*height*3);
		ch->owned = 1;
	}
}

void
channel_deinit(Channel *ch)
{
	pthread_mutex_destroy(&ch->mutex);
	if (ch->owned) free(ch->pixbuf);
	ch->pixbuf = NULL;
	ch->owned = 0;
}

uint8_t*
channel_get_pixels(Channel *ch)
{
	pthread_mutex_lock(&ch->mutex);
	return ch->pixbuf;
}

void
channel_release_pixels(Channel *ch)
{
	pthread_mutex_unlock(&ch->mutex);
}
