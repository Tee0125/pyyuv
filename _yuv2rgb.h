
#ifndef __YUV2RGB_H__
#define __YUV2RGB_H__

#include <stdint.h>

uint8_t* yuv420_to_rgb24(uint8_t* frame, int w, int h);

#endif
