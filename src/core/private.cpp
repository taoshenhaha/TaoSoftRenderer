#include "private.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
void private_blit_rgb(FrameBuffer *src, image_t *dst)
{
    int width = dst->width;
    int height = dst->height;
    int r, c;

    assert(src->mWidth == dst->width && src->mHeight == dst->height);
    assert(dst->format == format_t::FORMAT_LDR && dst->channels == 4);

    for (r = 0; r < height; r++) {
        for (c = 0; c < width; c++) {
            int flipped_r = height - 1 - r;
            int src_index = r * width + c;
            int dst_index = (flipped_r * width + c) * 4;
            Vec4<float>  src_pixel = src->mColorBuffer[src_index];
            unsigned char *dst_pixel = &dst->ldr_buffer[dst_index];
            dst_pixel[0] = src_pixel.x;  /* red */
            dst_pixel[1] = src_pixel.y;  /* green */
            dst_pixel[2] = src_pixel.z;  /* blue */
            dst_pixel[3] = src_pixel.w;  /* alpha */
        }
    }
}

const char *private_get_extension(const char *filename) {
    const char *dot_pos = strrchr(filename, '.');
    return dot_pos == NULL ? "" : dot_pos + 1;
}