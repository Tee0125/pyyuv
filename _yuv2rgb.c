
#include "_yuv2rgb.h"

#include <stdio.h>
#include <stdlib.h>


#define clip255(a) ((a>255)?255:((a<0)?0:a))

uint8_t* yuv420_to_rgb24(uint8_t* frame, int w, int h)
{
    uint8_t* y = frame;
    uint8_t* u = y + w*h;
    uint8_t* v = u + ((w+1)/2)*((h+1)/2);

    uint8_t* rgb = malloc(sizeof(uint8_t)*w*h*3);
    uint8_t* dst = rgb;

    int c, d, e;
    int i, j;
    int offset;

    int stride = w*3;

    if (dst == NULL)
        return NULL;

    for (j = 0 ; j < h ; j++)
    {
        offset = 0;

        for (i = 0 ; i < w ; i++)
        {
            c = y[(i   )] -  16;
            d = u[(i>>1)] - 128;
            e = v[(i>>1)] - 128;

            dst[offset+0] = clip255((298 * c           + 409 * e + 128) >> 8);
            dst[offset+1] = clip255((298 * c - 100 * d - 208 * e + 128) >> 8);
            dst[offset+2] = clip255((298 * c + 516 * d           + 128) >> 8);

            offset += 3;
        }

        dst += stride;

        y += w;
        if (j&1)
        {
            u += (w+1)>>1;
            v += (w+1)>>1;
        }
    }

    return rgb;
}
