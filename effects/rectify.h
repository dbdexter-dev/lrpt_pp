#ifndef rectify_h
#define rectify_h

#include <stdint.h>
#include "image.h"

#define M2_SCAN_ANGLE 104*M_PI/180
#define M2_ALTITUDE 820

void rectify_m2(Image *dst, Image *src);
void rectify(Image *dst, Image *src, float altitude, float scan_angle);

#endif
