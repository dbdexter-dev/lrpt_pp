#include <stdio.h>
#include <pthread.h>
#include "composite.h"
#include "image.h"
#include "utils.h"

typedef struct {
	void (*kernel)(Image*, Image*);
	void (*callback)();
	Image *dst, *src;
} Args;

static void* composite_apply_kernel(void *args);

static Image _original, _rectified, _sharpened, _enhanced;
static Image *_to_rectify, *_to_sharpen, *_to_enhance, *_output;
static int initialized = 0;
static Args _a;

void
composite_init(uint8_t *base, int width, int height, int rowstride, int bpp)
{
	/* If already initialized, free the underlying buffers first */
	if (initialized) {
		composite_deinit();
	}

	image_init(&_original, width, height, rowstride, bpp, base);
	image_init(&_rectified, width, 1, _original.rowstride, 24, NULL);
	image_init(&_sharpened, width, 1, _original.rowstride, 24, NULL);
	image_init(&_enhanced, width, 1, _original.rowstride, 24, NULL);

	_to_rectify = _to_sharpen = _to_enhance = _output = &_original;

	initialized = 1;
}

void
composite_deinit()
{
	if (initialized) {
		image_deinit(&_original);
		image_deinit(&_rectified);
		image_deinit(&_sharpened);
		image_deinit(&_enhanced);
	}
}

void
composite_set_rectify(int rectify, void (*callback)())
{
	pthread_t tid;

	if (rectify) {
		_a.kernel = rectify_m2;
		_a.callback = callback;
		_a.src = &_original;
		_a.dst = &_rectified;

		_to_sharpen = &_rectified;
		if (_to_enhance != _to_sharpen) _to_enhance = &_rectified;
		pthread_create(&tid, NULL, composite_apply_kernel, (void*)&_a);
	} else {
		_to_sharpen = &_original;
		if (_to_enhance == _to_sharpen) _to_enhance = &_original;
		if (callback) callback();
	}

}

void
composite_set_sharpen(int sharpen, void (*callback)())
{
	pthread_t tid;

	if (sharpen) {
		_a.kernel = sharpen_default;
		_a.callback = callback;
		_a.src = _to_sharpen;
		_a.dst = &_sharpened;

		_to_enhance = &_sharpened;

		pthread_create(&tid, NULL, composite_apply_kernel, (void*)&_a);
	} else {
		_to_enhance = _to_sharpen;
		if (callback) callback();
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
	_a.src = _to_enhance;
	_a.dst = &_enhanced;

	if (_a.src->width != _a.dst->width) {
		image_deinit(_a.dst);
		image_init(_a.dst, _a.src->width, _a.src->height, _a.src->rowstride, 24, NULL);
	}

	printf("Enhancing %p to %p\n", _a.src, _a.dst);

	pthread_create(&tid, NULL, composite_apply_kernel, (void*)&_a);
	_output = &_enhanced;
}

void*
composite_apply_kernel(void *args)
{
	const Args *a = (Args*)args;

	printf("Applying kernel %p\n", a->kernel);

	pthread_mutex_lock(&a->dst->mutex);
	if (a->kernel) a->kernel(a->dst, a->src);
	pthread_mutex_unlock(&a->dst->mutex);

	printf("Done\n");

	if (a->callback) a->callback();
	return NULL;
}


uint8_t*
composite_get_pixels()
{
	if (initialized) return image_get_pixels(_output);
	return NULL;
}

void
composite_release_pixels()
{
	if (initialized) image_release_pixels(_output);
}

int
composite_get_width()
{
	return _output->width;
}
int
composite_get_height()
{
	return _output->height;
}
int
composite_get_rowstride()
{
	return _output->rowstride;
}
