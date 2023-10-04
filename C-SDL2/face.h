#ifndef FACE_H
#define FACE_H

#include "main.h"

struct Face {
    SDL_Renderer *renderer;
    SDL_Texture **image_array;
    int image_length;
    SDL_Rect image_rect;
    int image_index;
    int theme;
};

bool face_new(struct Face **face, SDL_Renderer *renderer);
void face_free(struct Face **face);
SDL_Rect face_rect(struct Face *f);
void face_set_theme(struct Face *f, int theme);
void face_reset(struct Face *f, int window_width, int scale);
bool face_mouse_click(struct Face *f, int x, int y, bool down);
void face_default(struct Face *f);
void face_won(struct Face *f);
void face_lost(struct Face *f);
void face_question(struct Face *f);
bool face_draw(struct Face *f); 

#endif
