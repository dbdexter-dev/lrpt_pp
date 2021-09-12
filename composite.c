#include <stdio.h>
#include <pthread.h>
#include "composite.h"
#include "image.h"

typedef struct {
	void (*kernel)(Image*, Image*);
	void (*callback)();
	Image *dst, *src;
} Args;

static void* composite_apply_kernel(void *args);

static Image _images[3];
static int _unenhanced_image = 0;
static int _output_image = 0;
static int initialized = 0;
static Args _a;

void
composite_init(uint8_t *base, int width, int height, int rowstride, int bpp)
{
	/* If already initialized, free the underlying buffers first */
	if (initialized) {
		composite_deinit();
	}

	image_init(&_images[0], width, height, rowstride, bpp, base);
	image_init(&_images[1], width, height, _images[0].rowstride, 24, NULL);
	_output_image = 1;
	_unenhanced_image = 0;
	initialized = 1;
}

void
composite_deinit()
{
	int i;

	for (i=0; i<_output_image; i++) {
		image_deinit(&_images[i]);
	}
}

void
composite_set_enhancement(Enhancement e, void(*callback)())
{
	void (*kernel)(Image*, Image*) = enhance_none;
	pthread_t tid;

	switch (e) {
		case NONE:
			break;
		case VISIBLE:
			kernel = enhance_122;
			break;
		case VEGETATION:
			kernel = enhance_vegetation;
			break;
		case VEG_IR:
			kernel = enhance_211;
			break;
		case THERMAL:
			kernel = enhance_thermal;
			break;
		case HVC:
			kernel = enhance_hvc;
			break;
		case HVC_PRECIP:
			kernel = enhance_hvc_precip;
			break;
		default:
			printf("Unknown enhancement: %d\n", e);
			break;
	}

	_a.kernel = kernel;
	_a.callback = callback;
	_a.src = &_images[_unenhanced_image];
	_a.dst = &_images[_output_image];

	printf("Enhancing %p to %p\n", _a.src, _a.dst);

	pthread_create(&tid, NULL, composite_apply_kernel, (void*)&_a);
}

void*
composite_apply_kernel(void *args)
{
	const Args *a = (Args*)args;

	pthread_mutex_lock(&a->dst->mutex);
	a->kernel(a->dst, a->src);
	pthread_mutex_unlock(&a->dst->mutex);

	a->callback();
	return NULL;
}


uint8_t*
composite_get_pixels()
{
	return image_get_pixels(&_images[_output_image]);
}

void
composite_release_pixels()
{
	image_release_pixels(&_images[_output_image]);
}

int
composite_get_width()
{
	return _images[_output_image].width;
}
int
composite_get_height()
{
	return _images[_output_image].height;
}
int
composite_get_rowstride()
{
	return _images[_output_image].rowstride;
}
