#ifndef MINES_H
#define MINES_H

#include "main.h"

struct Mines {
    SDL_Renderer *renderer;
    SDL_Texture **background_array;
    int background_length;
    SDL_Rect background_rect;
    SDL_Texture **digits_array;
    int digits_length;
    SDL_Rect digits_rect;
    int mine_count;
    int digits[3];
    int digit_x;
    int digit_theme;
    int back_theme;
};

bool mines_new(struct Mines **mines, SDL_Renderer *renderer);
void mines_free(struct Mines **mines);
void mines_set_theme(struct Mines *m, int theme);
void mines_reset(struct Mines *m, int mine_count, int scale);
void mines_increment(struct Mines *m);
void mines_decrement(struct Mines *m); 
void mines_digits(struct Mines *m); 
bool mines_draw(struct Mines *m); 

#endif
