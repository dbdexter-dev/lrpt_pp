#include <stdio.h>
#include <pthread.h>
#include "composite.h"
#include "image.h"

typedef struct {
	void (*kernel)(int, int, int, uint8_t*, uint8_t*);
	void (*callback)();
} Args;

static void* composite_apply_kernel(void *args);

static Image _original, _composite;
static int initialized = 0;
static Args _a;

void
composite_init(uint8_t *base, int width, int height, int rowstride)
{
	/* If already initialized, free the underlying buffers first */
	if (initialized) {
		composite_deinit();
	}

	image_init(&_original, width, height, rowstride, base);
	image_init(&_composite, width, height, rowstride, NULL);
	initialized = 1;
}

void
composite_deinit()
{
	image_deinit(&_original);
	image_deinit(&_composite);
}

void
composite_set_enhancement(Enhancement e, void(*callback)())
{
	void (*kernel)(int, int, int, uint8_t*, uint8_t*) = enhancement_none;
	pthread_t tid;

	switch (e) {
		case NONE:
			break;
		case VISIBLE:
			kernel = enhancement_122;
			break;
		case VEGETATION:
			kernel = enhancement_vegetation;
			break;
		case VEG_IR:
			kernel = enhancement_211;
			break;
		case THERMAL:
			kernel = enhancement_thermal;
			break;
		case HVC:
			kernel = enhancement_hvc;
			break;
		case HVC_PRECIP:
			kernel = enhancement_hvc_precip;
			break;
		default:
			printf("Unknown enhancement: %d\n", e);
			break;
	}

	_a.kernel = kernel;
	_a.callback = callback;

	pthread_create(&tid, NULL, composite_apply_kernel, (void*)&_a);
}

void*
composite_apply_kernel(void *args)
{
	uint8_t *src, *dst;
	int width, height, rowstride;
	const Args *a = (Args*)args;

	printf("Applying kernel %p\n", a->kernel);

	pthread_mutex_lock(&_composite.mutex);

	src = _original.pixbuf;
	dst = _composite.pixbuf;
	width = _original.width;
	height = _original.height;
	rowstride = _original.rowstride;

	a->kernel(width, height, rowstride, src, dst);

	pthread_mutex_unlock(&_composite.mutex);

	a->callback();
	return NULL;
}


uint8_t*
composite_get_pixels()
{
	return image_get_pixels(&_composite);
}

void
composite_release_pixels()
{
	image_release_pixels(&_composite);
}

int
composite_get_width()
{
	return _composite.width;
}
int
composite_get_height()
{
	return _composite.height;
}
int
composite_get_rowstride()
{
	return _composite.rowstride;
}
