#ifndef BORDER_H
#define BORDER_H

#include "main.h"

struct Border {
    SDL_Renderer *renderer;
    SDL_Texture **image_array;
    int image_length;
    SDL_Rect rect;
    int rows;
    int columns;
    int border_size;
    int piece_size;
    int offset_x;
    int theme;
};

bool border_new(struct Border **border, SDL_Renderer *renderer);
void border_free(struct Border **border);
void border_set_theme(struct Border *b, int theme);
void border_reset(struct Border *b, int rows, int columns, int scale);
bool border_draw(struct Border *b); 

#endif
