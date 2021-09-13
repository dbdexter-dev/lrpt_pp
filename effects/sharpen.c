#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include "sharpen.h"
#include "utils.h"

#ifndef M_PI
#define M_PI 3.1415926536
#endif

static void gaussian_kernel(float *dst, int width, int height, float sigma);
static void kernel_integrate_h(float *dst, const float *src, int width, int height);
static void kernel_integrate_v(float *dst, const float *src, int width, int height);
static void convolve_1d_h(uint8_t *dst, const uint8_t *src, const float *kernel, int width, int height, int rowstride, int kernel_len);
static void convolve_1d_v(uint8_t *dst, const uint8_t *src, const float *kernel, int width, int height, int rowstride, int kernel_len);

void
sharpen(Image *dst, Image *src, float radius, float amount)
{
	float *ker, *ker_v, *ker_h;
	uint8_t *tmp_pixbuf;
	float px;
	float amount_base, amount_blur;
	int i, j;

	/* Compute weights for base and blur images */
	amount_blur = -amount;
	amount_base = 1-amount_blur;

	/* Generate gaussian blur kernel */
	ker = malloc(sizeof(*ker) * radius * radius);
	gaussian_kernel(ker, radius, radius, 1);

	/* Separate kernel into two components */
	ker_h = malloc(sizeof(*ker_h) * radius);
	ker_v = malloc(sizeof(*ker_v) * radius);
	kernel_integrate_v(ker_h, ker, radius, radius);
	kernel_integrate_h(ker_v, ker, radius, radius);

	/* Generate blurred image */
	image_resize_match(dst, src);
	tmp_pixbuf = malloc(sizeof(*tmp_pixbuf) * src->rowstride * src->height);

	/* Blur */
	convolve_1d_h(tmp_pixbuf, src->pixbuf, ker_h, src->width, src->height, src->rowstride, radius);
	convolve_1d_v(dst->pixbuf, tmp_pixbuf, ker_v, src->width, src->height, src->rowstride, radius);

	/* Unsharp */
	for (i=0; i<dst->height; i++) {
		for (j=0; j<dst->rowstride; j++) {
			px = amount_base * src->pixbuf[i*src->rowstride + j]
			   + amount_blur * dst->pixbuf[i*dst->rowstride + j];
			dst->pixbuf[i*dst->rowstride + j] = MAX(0, MIN(255, px));
		}
	}

	/* Cleanup */
	free(tmp_pixbuf);
	free(ker);
	free(ker_h);
	free(ker_v);
}

void
sharpen_default(Image *dst, Image *src)
{
	sharpen(dst, src, M2_RADIUS, M2_AMOUNT);
}

/* Static functions {{{ */
static void
convolve_1d_h(uint8_t *dst, const uint8_t *src, const float *kernel, int width, int height, int rowstride, int kernel_len)
{
	int row, col, k;
	int lr, lc;
	float px;

	for (row=0; row<height; row++) {
		for (col=0; col<rowstride; col++) {
			px = 0;
			for (k=0; k<kernel_len; k++) {

				lr = row;
				lc = MAX(0, MIN(rowstride-1, col + 3 * (k - kernel_len/2)));

				px += src[lr * rowstride + lc] * kernel[k];
			}

			dst[row * rowstride + col] = MAX(0, MIN(255, px));
		}
	}
}

static void
convolve_1d_v(uint8_t *dst, const uint8_t *src, const float *kernel, int width, int height, int rowstride, int kernel_len)
{
	int row, col, k;
	int lr, lc;
	float px;

	for (row=0; row<height; row++) {
		for (col=0; col<rowstride; col++) {
			px = 0;
			for (k=0; k<kernel_len; k++) {

				lr = MAX(0, MIN(height-1, row + k - kernel_len/2));
				lc = col;

				px += src[lr * rowstride + lc] * kernel[k];
			}

			dst[row * rowstride + col] = MAX(0, MIN(255, px));
		}
	}
}

static void
gaussian_kernel(float *dst, int width, int height, float sigma)
{
	int x, y;

	for (y = 0; y < height; y++) {
		for (x = 0; x < height; x++) {
			const int lx = x - width/2;
			const int ly = y - height/2;
			const float sigma_2 = sigma * sigma;
			const float scale = 1.0 / (2.0 * M_PI * sigma_2);
			const float e_pow = expf(-(lx*lx + ly*ly) / (2 * sigma_2));

			dst[y * width + x] = scale * e_pow;
		}
	}
}

static void
kernel_integrate_h(float *dst, const float *src, int width, int height)
{
	int i, j;
	float sum;

	/* Integrate horizontally */
	for (i=0; i<width; i++) {
		dst[i] = 0.0;

		for (j=0; j<height; j++) {
			dst[i] += src[i * height + j];
		}
	}

	/* Normalize */
	sum = 0.0;
	for (i=0; i<width; i++) {
		sum += dst[i];
	}
	for (i=0; i<width; i++) {
		dst[i] /= sum;
	}
}

static void
kernel_integrate_v(float *dst, const float *src, int width, int height)
{
	int i, j;
	float sum;

	/* Integrate horizontally */
	for (j=0; j<height; j++) {
		dst[j] = 0.0;

		for (i=0; i<width; i++) {
			dst[i] += src[i * height + j];
		}
	}

	/* Normalize */
	sum = 0.0;
	for (i=0; i<height; i++) {
		sum += dst[i];
	}
	for (i=0; i<height; i++) {
		dst[i] /= sum;
	}
}
/* }}} */
