#include "game.h"
#include "initialize.h"

bool game_new(struct Game **game) {
    *game = calloc(1, sizeof(struct Game));
    if (!*game) {
        fprintf(stderr, "Error in calloc of new game.\n");
        return true;
    }

    struct Game *g = *game;

    if (game_initilize(g)) {
        return true;
    }

    if (board_new(&g->board, g->renderer)) {
        return true;
    }

    if (border_new(&g->border, g->renderer)) {
        return true;
    }

    if (mines_new(&g->mines, g->renderer)) return true;

    if (clock_new(&g->clock, g->renderer)) return true;

    if (face_new(&g->face, g->renderer)) return true;

    if (fps_new(&g->fps)) {
        return true;
    }

    g->rows = 9;
    g->columns = 9;
    g->scale = 2;
    board_set_theme(g->board, 5);
    face_set_theme(g->face, 2);

    char size_str[] = "Tiny";
    g->size_str = calloc(1, sizeof(size_str));
    if (!g->size_str) {
        fprintf(stderr, "Error in calloc of size string.\n");
        return true;
    }
    snprintf(g->size_str, sizeof(size_str), "%s", size_str);

    g->difficulty = 0.095;

    char diff_str[] = "Easy";
    g->diff_str = calloc(1, sizeof(diff_str));
    if (!g->diff_str) {
        fprintf(stderr, "Error in calloc of difficluty string.\n");
        return true;
    }
    snprintf(g->diff_str, sizeof(diff_str), "%s", diff_str);

    if (game_reset(g)) {
        return true;
    }

    return false;
}

void game_free(struct Game **game) {
    if (*game) {
        board_free(&(*game)->board);
        border_free(&(*game)->border);
        mines_free(&(*game)->mines);
        clock_free(&(*game)->clock);
        face_free(&(*game)->face);
        fps_free(&(*game)->fps);

        free((*game)->size_str);
        (*game)->size_str = NULL;
        free((*game)->diff_str);
        (*game)->diff_str = NULL;
        SDL_DestroyRenderer((*game)->renderer);
        (*game)->renderer = NULL;
        SDL_DestroyWindow((*game)->window);
        (*game)->window = NULL;
        SDL_Quit();
        free(*game);
        *game = NULL;
    }
}

void game_set_theme(struct Game *g, unsigned theme) {
    switch (theme) {
        case 1:
            board_set_theme(g->board, 1);
            mines_set_theme(g->mines, 1);
            clock_set_theme(g->clock, 1);
            border_set_theme(g->border, 1);
            face_set_theme(g->face, 1);
            break;
        case 2:
            board_set_theme(g->board, 2);
            mines_set_theme(g->mines, 1);
            clock_set_theme(g->clock, 1);
            border_set_theme(g->border, 1);
            face_set_theme(g->face, 1); 
            break;
        case 3:
            board_set_theme(g->board, 3);
            mines_set_theme(g->mines, 1);
            clock_set_theme(g->clock, 1);
            border_set_theme(g->border, 1);
            face_set_theme(g->face, 1);
            break;
        case 4:
            board_set_theme(g->board, 4);
            mines_set_theme(g->mines, 1);
            clock_set_theme(g->clock, 1);
            border_set_theme(g->border, 1);
            face_set_theme(g->face, 2);
            break;
        case 5:
            board_set_theme(g->board, 5);
            mines_set_theme(g->mines, 1);
            clock_set_theme(g->clock, 1);
            border_set_theme(g->border, 1);
            face_set_theme(g->face, 2);
            break;
        case 6:
            board_set_theme(g->board, 6);
            mines_set_theme(g->mines, 1);
            clock_set_theme(g->clock, 1);
            border_set_theme(g->border, 1);
            face_set_theme(g->face, 2);
            break;
        case 7:
            board_set_theme(g->board, 7);
            mines_set_theme(g->mines, 2);
            clock_set_theme(g->clock, 2);
            border_set_theme(g->border, 2);
            face_set_theme(g->face, 3);
            break;
        case 8:
            board_set_theme(g->board, 8);
            mines_set_theme(g->mines, 2);
            clock_set_theme(g->clock, 2);
            border_set_theme(g->border, 2);
            face_set_theme(g->face, 3);
            break;
        default:
            break;
    }
}

bool game_set_difficulty(struct Game *g, double difficulty, const char *diff_str) {
    g->difficulty = difficulty;

    int length = snprintf(NULL, 0, "%s", diff_str) + 1;
    if (g->diff_str) {
        free(g->diff_str);
        g->diff_str = NULL;
    }

    g->diff_str = calloc(1, sizeof(char) * (unsigned)length);
    if (!g->diff_str) {
        fprintf(stderr, "Error in calloc of difficulty string.\n");
        return true;
    }

    snprintf(g->diff_str, (size_t)length, "%s", diff_str);
    
    if (game_reset(g)) {
        return true;
    }

    return false;
}

bool game_set_size(struct Game *g, int rows, int columns, int scale, const char *size_str) {
    g->rows = rows;
    g->columns = columns;
    g->scale = scale;

    int length = snprintf(NULL, 0, "%s", size_str) + 1;
    if (g->size_str) {
        free(g->size_str);
        g->size_str = NULL;
    }

    g->size_str = calloc(1, sizeof(char) * (unsigned)length);
    if (!g->size_str) {
        fprintf(stderr, "Error in calloc of size string.\n");
        return true;
    }

    snprintf(g->size_str, (size_t)length, "%s", size_str);
    
    if (game_reset(g)) {
        return true;
    }

    return false;
}

void game_set_window(struct Game *g) {
    g->window_width = ((g->columns + 2) * PIECE_SIZE - LEFT_OFFSET - RIGHT_OFFSET) * g->scale;
    int window_height = ((2 * BORDER_SIZE) + g->rows * PIECE_SIZE - BOTTOM_OFFSET) * g->scale;
    SDL_SetWindowSize(g->window, g->window_width, window_height);

    SDL_SetWindowPosition(g->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    int length = snprintf(NULL, 0, "%s - %s - %s", WINDOW_TITLE, g->size_str, g->diff_str) + 1;
    char title_str[length];

    snprintf(title_str, (size_t)length, "%s - %s - %s", WINDOW_TITLE, g->size_str, g->diff_str);

    SDL_SetWindowTitle(g->window, title_str);
}

bool game_reset(struct Game *g) {
    game_set_window(g);

    g->mine_count = (int)((g->rows * g->columns * g->difficulty) + 0.5);

    if (board_reset(g->board, (unsigned)g->rows, (unsigned)g->columns, g->scale, g->mine_count)) {
        return true;
    }

    border_reset(g->border, g->rows, g->columns, g->scale);

    mines_reset(g->mines, g->mine_count, g->scale);

    clock_reset(g->clock, g->window_width, g->scale);

    face_reset(g->face, g->window_width, g->scale);


    g->playing = true;

    return false;
}

bool game_mouse_down(struct Game *g, int x, int y, Uint8 button) {
    if (button == SDL_BUTTON_LEFT) {
        face_mouse_click(g->face, x, y, true);
    }

    if (g->playing) {
        if (board_mouse_down(g->board, x, y, button)) {
            face_question(g->face);
        }
    }

    return false;
}

bool game_mouse_up(struct Game *g, int x, int y, Uint8 button) {
    if (button == SDL_BUTTON_LEFT) {
        if (face_mouse_click(g->face, x, y, false)) {
            if (game_reset(g)) {
                return true;
            }
        }
    }
    
    if (g->playing) {
        int game_status;
        int mines_mark;
        board_mouse_up(g->board, x, y, button, &game_status, &mines_mark);

        if (mines_mark == 1) {
            mines_increment(g->mines);
        } else if (mines_mark == -1) {
            mines_decrement(g->mines);
        }

        if (game_status == 1) {
            face_won(g->face);
            g->playing = false;
        } else if (game_status == -1) {
            face_lost(g->face);
            g->playing = false;
        } else {
            face_default(g->face);
        }
    }

    return false;
}

bool game_run(struct Game *g) {
    while (true) {
        while (SDL_PollEvent(&g->event)) {
            switch (g->event.type) {
                case SDL_QUIT:
                    return false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (game_mouse_down(g, g->event.button.x, g->event.button.y, g->event.button.button)) {
                        return true;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (game_mouse_up(g, g->event.button.x, g->event.button.y, g->event.button.button)) {
                        return true;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (g->event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                            if (g->playing) {
                                g->playing = false;
                            } else {
                                return false;
                            }
                            break;
                        case SDL_SCANCODE_1:
                            game_set_theme(g, 1);
                            break;
                        case SDL_SCANCODE_2:
                            game_set_theme(g, 2);
                            break;
                        case SDL_SCANCODE_3:
                            game_set_theme(g, 3);
                            break;
                        case SDL_SCANCODE_4:
                            game_set_theme(g, 4);
                            break;
                        case SDL_SCANCODE_5:
                            game_set_theme(g, 5);
                            break;
                        case SDL_SCANCODE_6:
                            game_set_theme(g, 6);
                            break;
                        case SDL_SCANCODE_7:
                            game_set_theme(g, 7);
                            break;
                        case SDL_SCANCODE_8:
                            game_set_theme(g, 8);
                            break;
                        case SDL_SCANCODE_A:
                            if (game_set_difficulty(g, 0.095, "Easy")) return true;
                            break;
                        case SDL_SCANCODE_S:
                            if (game_set_difficulty(g, 0.13, "Medium")) return true;
                            break;
                        case SDL_SCANCODE_D:
                            if (game_set_difficulty(g, 0.165, "Hard")) return true;
                            break;
                        case SDL_SCANCODE_F:
                            if (game_set_difficulty(g, 0.2, "Very Hard")) return true;
                            break;
                        case SDL_SCANCODE_Q:
                            if (game_set_size(g, 9, 9, 2, "Tiny")) return true;
                            break;
                        case SDL_SCANCODE_W:
                            if (game_set_size(g, 16, 16, 2, "Small")) return true;
                            break;
                        case SDL_SCANCODE_E:
                            if (game_set_size(g, 16, 30, 2, "Medium")) return true;
                            break;
                        case SDL_SCANCODE_R:
                            if (game_set_size(g, 20, 40, 2, "Large")) return true;
                            break;
                        case SDL_SCANCODE_T:
                            if (game_set_size(g, 40, 80, 1, "Huge")) return true;
                            break;
                        case SDL_SCANCODE_P:
                            fps_toggle_display(g->fps);
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        
        if (g->playing) {
            clock_update(g->clock);
        }

        if (SDL_RenderClear(g->renderer)) {
            fprintf(stderr, "Error while clearing renderer: %s\n", SDL_GetError());
            return true;
        }

        if (border_draw(g->border)) {
            return true;
        }

        if (board_draw(g->board)) {
            return true;
        }

        if (mines_draw(g->mines)) return true;

        if (clock_draw(g->clock)) return true;

        if (face_draw(g->face)) return true;

        SDL_RenderPresent(g->renderer);

        g->delta_time = fps_update(g->fps);
    }
    
    return false;
}
