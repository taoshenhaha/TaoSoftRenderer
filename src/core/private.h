#pragma once

#include "image.h"
#include "../graphics/Graphics.h"

void private_blit_rgb(FrameBuffer *src, image_t *dst);
/* misc functions */
const char *private_get_extension(const char *filename);