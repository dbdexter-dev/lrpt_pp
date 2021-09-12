#ifndef enhancements_h
#define enhancements_h

#include <stdint.h>
#include "image.h"

typedef enum {
	NONE,
	VISIBLE,        /* RGB122 */
	VEGETATION,     /* Green = vegetation */
	VEG_IR,         /* Red = vegetation */
	HVC,
	HVC_PRECIP,
	THERMAL,
} Enhancement;

char *enhancement_str(Enhancement e);

void enhance_none(Image *dst, Image *src);
void enhance_122(Image *dst, Image *src);
void enhance_211(Image *dst, Image *src);
void enhance_vegetation(Image *dst, Image *src);
void enhance_thermal(Image *dst, Image *src);
void enhance_hvc(Image *dst, Image *src);
void enhance_hvc_precip(Image *dst, Image *src);

#endif
