#ifndef BOARD_H
#define BOARD_H

#include "main.h"

struct Board {
    SDL_Renderer *renderer;
    SDL_Texture **image_array;
    unsigned int image_length;
    SDL_Rect image_rect;
    unsigned **front_array;
    unsigned **back_array;
    unsigned rows;
    unsigned columns;
    int offset_x;
    int offset_y;
    SDL_Rect board_rect;
    bool pressed;
    struct Node *check_head;
    unsigned theme;
};

struct Node {
    struct Node *next;
    struct Node *previous;
    int row;
    int column;
};

bool board_new(struct Board **board, SDL_Renderer *renderer);
void board_free(struct Board **board);
void board_free_arrays(struct Board *b); 
bool board_calloc_arrays(struct Board *b, unsigned rows, unsigned columns); 
SDL_Rect board_rect(struct Board *b);
void board_set_theme(struct Board *b, unsigned theme);
bool board_reset(struct Board *b, unsigned rows, unsigned columns, int scale, int mines);
bool board_mouse_down(struct Board *b, int x, int y, Uint8 button); 
bool board_mouse_up(struct Board *b, int x, int y, Uint8 button, int *game_status, int *mines_mark);
bool board_push_check(struct Board *b, int row, int column);
void board_remove_check(struct Board *b, struct Node *node);
void board_free_check(struct Board *b);
void board_run_check(struct Board *b);
bool board_game_won(struct Board *b);
bool board_draw(struct Board *b); 

#endif
