#ifndef CLOCK_H
#define CLOCK_H

#include "main.h"

struct Clock {
    SDL_Renderer *renderer;
    SDL_Texture **background_array;
    unsigned background_length;
    SDL_Rect background_rect;
    SDL_Texture **digits_array;
    unsigned int digits_length;
    SDL_Rect digits_rect;
    int seconds;
    Uint32 last_time;
    int digits[3];
    int digit_x;
    int digit_theme;
    int back_theme;
};

bool clock_new(struct Clock **clock, SDL_Renderer *renderer);
void clock_free(struct Clock **clock);
void clock_set_theme(struct Clock *c, int theme);
void clock_reset(struct Clock *c, int window_width, int scale);
void clock_digits(struct Clock *c); 
void clock_update(struct Clock *c); 
bool clock_draw(struct Clock *c); 

#endif
