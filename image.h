#ifndef image_h
#define image_h

#include <pthread.h>
#include <stdint.h>

typedef struct {
	uint8_t *pixbuf;
	int width, height, rowstride;
	int owned;
	pthread_mutex_t mutex;
} Image;

void image_init(Image *ch, int width, int height, int rowstride, int bpp, uint8_t *pixbuf);
void image_deinit(Image *ch);

uint8_t* image_get_pixels(Image *ch);
void image_release_pixels(Image *ch);

#endif
