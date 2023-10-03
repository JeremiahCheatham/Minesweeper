#ifndef GAME_H
#define GAME_H

#include "main.h"
#include "board.h"
#include "border.h"
#include "mines.h"
#include "clock.h"
#include "face.h"
#include "fps.h"

struct Game {
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    struct Board *board;
    struct Border *border;
    struct Mines *mines;
    struct Clock *clock;
    struct Face *face;
    struct Fps *fps;
    int rows;
    int columns;
    int scale;
    int window_width;
    int mine_count;
    char *size_str;
    double difficulty;
    char *diff_str;
    int board_theme;
    bool playing;
    double delta_time;
};

bool game_new(struct Game **game);
void game_free(struct Game **game);
void game_set_theme(struct Game *g, unsigned theme); 
bool game_set_difficulty(struct Game *g, double difficulty, const char *diff_str); 
bool game_set_size(struct Game *g, int rows, int columns, int scale, const char *size_str);
void game_set_window(struct Game *g);
bool game_reset(struct Game *g);
bool game_mouse_down(struct Game *g, int x, int y, Uint8 button);
bool game_mouse_up(struct Game *g, int x, int y, Uint8 button); 
bool game_run(struct Game *g);

#endif
