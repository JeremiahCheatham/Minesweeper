#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SDL_FLAGS (SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER)
#define WINDOW_TITLE "Minesweeper"
#define WINDOW_WIDTH 328
#define WINDOW_HEIGHT 414
#define TARGET_FPS 60
#define PIECE_SIZE 16
#define BORDER_SIZE 55
#define FACE_SIZE 26
#define FACE_TOP_OFFSET 15
#define DIGIT_BACK_WIDTH 41
#define DIGIT_BACK_HEIGHT 25
#define DIGIT_WIDTH 13
#define DIGIT_HEIGHT 23
#define MINES_LEFT_OFFSET 15
#define MINES_TOP_OFFSET 15
#define CLOCK_RIGHT_OFFSET 11
#define LEFT_OFFSET 4
#define RIGHT_OFFSET 8
#define BOTTOM_OFFSET 47

// Length of array macro.
#define LEN(x) (sizeof(x)/sizeof(x[0]))

#endif
