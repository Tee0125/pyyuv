/*
 * Copyright (c) 2019 Tee Jung
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "_yuv.h"
#include "_yuv2rgb.h"

#include <stdio.h>
#include <stdlib.h>


yuv_ctx_t* yuv_open(char* filename, int width, int height)
{
    yuv_ctx_t* ctx;

    int file_size;
    int frame_size;
    int frame_cnt;

    /* alloc yuv context */
    ctx = (yuv_ctx_t*)malloc(sizeof(yuv_ctx_t));
    if (ctx == NULL)
    {
        fprintf(stderr, "Could not allocate context memory\n");
        return NULL;
    }

    FILE* fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Could not open file: %s\n", filename);
        free(ctx);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    fprintf(stderr, "w=%d, h=%d\n", width, height);
    frame_size = width*height + (((width+1)/2)*((height+1)/2))*2;
    frame_cnt = file_size / frame_size;

    if (frame_cnt < 1)
    {
        fprintf(stderr, "No frame found\n");
        fclose(fp);
        free(ctx);
        return NULL;
    }

    ctx->fp = fp;
    ctx->frame_size = frame_size;

    ctx->frame_no = 0;
    ctx->frame_cnt = frame_cnt;

    ctx->width = width;
    ctx->height = height;

    ctx->frame = (uint8_t*)malloc(sizeof(uint8_t)*ctx->frame_size);

    return ctx;
}

int yuv_seek_frame(yuv_ctx_t* ctx, int frame_no)
{
    if (frame_no+1 >= ctx->frame_cnt)
        return 1;

    fseek(ctx->fp, ctx->frame_size * frame_no, SEEK_SET);
    ctx->frame_no = frame_no;

    return 0;
}

uint8_t* yuv_get_frame(yuv_ctx_t* ctx)
{
    if ((ctx->frame_no+1) >= ctx->frame_cnt)
        return NULL;

    int n = fread(ctx->frame, 1, ctx->frame_size, ctx->fp);
    fprintf(stderr, "frame_size: %d, n: %d\n", ctx->frame_size, n);
    ctx->frame_no++;

    return yuv420_to_rgb24(ctx->frame, ctx->width, ctx->height);
}

int yuv_get_width(yuv_ctx_t* ctx)
{
    return ctx->width;
}

int yuv_get_height(yuv_ctx_t* ctx)
{
    return ctx->height;
}

void yuv_release_frame(yuv_ctx_t* ctx, uint8_t* frame)
{
    free(frame);
}

void yuv_close(yuv_ctx_t* ctx)
{
    if (ctx->fp != NULL)
        fclose(ctx->fp);

    if (ctx->frame != NULL)
        free(ctx->frame);

    free(ctx);
}

