#ifndef enhancements_h
#define enhancements_h

#include <stdint.h>

typedef enum {
	NONE,
	VISIBLE,        /* RGB122 */
	VEGETATION,     /* Green = vegetation */
	VEG_IR,         /* Red = vegetation */
	HVC,
	HVC_PRECIP,
	THERMAL,
} Enhancement;

void enhancement_none(int width, int height, int rowstride, uint8_t *dst, uint8_t *src);
void enhancement_122(int width, int height, int rowstride, uint8_t *dst, uint8_t *src);
void enhancement_211(int width, int height, int rowstride, uint8_t *dst, uint8_t *src);
void enhancement_vegetation(int width, int height, int rowstride, uint8_t *dst, uint8_t *src);
void enhancement_thermal(int width, int height, int rowstride, uint8_t *dst, uint8_t *src);
void enhancement_hvc(int width, int height, int rowstride, uint8_t *dst, uint8_t *src);
void enhancement_hvc_precip(int width, int height, int rowstride, uint8_t *dst, uint8_t *src);

#endif
