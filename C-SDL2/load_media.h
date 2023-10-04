#ifndef LOAD_MEDIA_H
#define LOAD_MEDIA_H

#include "main.h"

bool load_media_sheet(
    SDL_Renderer *renderer,
    SDL_Texture ***image_array,
    int *image_length,
    SDL_Rect *rect,
    const char* file_path,
    int height,
    int width
);
void load_media_free(SDL_Texture ***image_array, int image_length); 

#endif
