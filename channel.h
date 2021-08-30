#ifndef channel_h
#define channel_h

#include <pthread.h>
#include <stdint.h>

typedef struct {
	uint8_t *pixbuf;
	int width, height, rowstride;
	int owned;
	pthread_mutex_t mutex;
} Channel;


void channel_init(Channel *ch, int width, int height, int rowstride, uint8_t *pixbuf);
void channel_deinit(Channel *ch);

uint8_t* channel_get_pixels(Channel *ch);
void channel_release_pixels(Channel *ch);

#endif
