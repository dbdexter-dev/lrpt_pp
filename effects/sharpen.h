#ifndef sharpen_h
#define sharpen_h

#include "image.h"

#define M2_RADIUS 3
#define M2_AMOUNT 1

void sharpen(Image *dst, Image *src, float radius, float amount);
void sharpen_default(Image *dst, Image *src);

#endif
