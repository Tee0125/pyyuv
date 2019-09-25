
#ifndef __YUV_H__
#define __YUV_H__

#include <stdint.h>
#include <stdio.h>

typedef struct
{
    FILE* fp;
    
    int width;
    int height;

    uint8_t* frame;

    int frame_no;
    int frame_size;
    int frame_cnt;
} yuv_ctx_t;

yuv_ctx_t* yuv_open(char* filename, int width, int height);
void yuv_release_frame(yuv_ctx_t* ctx, uint8_t* frame);
void yuv_close(yuv_ctx_t* ctx);

int yuv_seek_frame(yuv_ctx_t* ctx, int frame_no);
uint8_t* yuv_get_frame(yuv_ctx_t* ctx);

int yuv_get_width(yuv_ctx_t* ctx);
int yuv_get_height(yuv_ctx_t* ctx);

#endif
