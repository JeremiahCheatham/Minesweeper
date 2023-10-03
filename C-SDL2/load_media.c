#include "load_media.h"

bool load_media_sheet(
    SDL_Renderer *renderer,
    SDL_Texture ***image_array,
    unsigned int *image_length,
    SDL_Rect *rect,
    const char* file_path,
    unsigned height,
    unsigned width)
{
    if (*image_array) {
        fprintf(stderr, "Error image array is not NULL!\n");
    }

   SDL_Surface *source_surf = IMG_Load(file_path);
    if (!source_surf) {
        fprintf(stderr, "Error creating the source surface: %s\n", SDL_GetError());
        return true;
    }

    unsigned max_rows = (unsigned int)source_surf->h / height;
    unsigned max_columns = (unsigned int)source_surf->w / width;

    *image_array = calloc(1, sizeof(SDL_Texture *) * max_rows * max_columns);
    if (!*image_array) {
        fprintf(stderr, "Error in calloc of image array.\n");
        SDL_FreeSurface(source_surf);
        return true;
    }

    SDL_Surface *target_surf = SDL_CreateRGBSurfaceWithFormat(0, (int)width, (int)height, 32, SDL_PIXELFORMAT_ARGB8888);
    if (!target_surf) {
        fprintf(stderr, "Error creating the target surface: %s\n", SDL_GetError());
        SDL_FreeSurface(source_surf);
        return true;
    }

    (*rect).w = (int)width;
    (*rect).h = (int)height;

    *image_length = 0;
    for (unsigned int row = 0; row < max_rows; row++) {
        (*rect).y = (int)(row * height);
        for (unsigned int column = 0; column < max_columns; column++) {
            (*rect).x = (int)(column * width);

            if (SDL_FillRect(target_surf, NULL, SDL_MapRGBA(target_surf->format, 0, 0, 0, 0))) {
                fprintf(stderr, "Error filling a rectangle to a surface: %s\n", SDL_GetError());
                return true;
            }

            if (SDL_BlitSurface(source_surf, rect, target_surf, NULL)) {
                fprintf(stderr, "Error blitting surface: %s\n", SDL_GetError());
                SDL_FreeSurface(source_surf);
                SDL_FreeSurface(target_surf);
                return true;
            }

            (*image_array)[*image_length] = SDL_CreateTextureFromSurface(renderer, target_surf);
            if (!(*image_array)[*image_length]) {
                fprintf(stderr, "Error creating a image texture: %s\n", SDL_GetError());
                SDL_FreeSurface(source_surf);
                SDL_FreeSurface(target_surf);
                return true;
            }
            (*image_length)++;

        }
    }
    
    SDL_FreeSurface(source_surf);
    SDL_FreeSurface(target_surf);

    return false;
}

