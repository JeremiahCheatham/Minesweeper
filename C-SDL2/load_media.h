#ifndef LOAD_MEDIA_H
#define LOAD_MEDIA_H

#include "main.h"

bool load_media_sheet(
    SDL_Renderer *renderer,
    SDL_Texture ***image_array,
    unsigned int *image_length,
    SDL_Rect *rect,
    const char* file_path,
    unsigned height,
    unsigned width
);

#endif
