#ifndef enhancements_h
#define enhancements_h

typedef enum {
	NONE,
	VISIBLE,        /* RGB122 */
	VEGETATION,     /* Green = vegetation */
	HVC,
	THERMAL,
} Enhancement;

void enhancement_set(Enhancement e);

#endif
