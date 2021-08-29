#ifndef composite_h
#define composite_h

#include "channel.h"
#include "effects/enhancements.h"

void composite_init(uint8_t *base_img, int width, int height);
void composite_deinit();

void composite_set_enhancement(Enhancement e, void(*callback)());

uint8_t* composite_get_pixels();
void composite_release_pixels();
int composite_get_width();
int composite_get_height();

#endif
