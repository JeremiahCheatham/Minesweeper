#include "board.h"
#include "load_media.h"

bool board_new(struct Board **board, SDL_Renderer *renderer) {
    *board = calloc(1, sizeof(struct Board));
    if (!*board) {
        fprintf(stderr, "Error in calloc of new board.\n");
        return true;
    }
    struct Board *b = *board;

    b->renderer = renderer;

   load_media_sheet(
        b->renderer,
        &b->image_array,
        &b->image_length,
        &b->image_rect,
        "images/board.png",
        PIECE_SIZE,
        PIECE_SIZE
    );
    
    return false;
}

void board_free(struct Board **board) {
    if (*board) {
        if ((*board)->image_array) {
            for (unsigned int i = 0; i < (*board)->image_length; i++) {
                if ((*board)->image_array[i]) {
                    SDL_DestroyTexture((*board)->image_array[i]);
                }
            }
            free((*board)->image_array);
        }

        board_free_arrays(*board);
        board_free_check(*board);

        (*board)->renderer = NULL;
        free(*board);
        *board = NULL;
    }
}

void board_free_arrays(struct Board *b) {
    if (b->front_array) {
        for (unsigned r = 0; r < b->rows; r++) {
            free(b->front_array[r]);
        }
        free(b->front_array);
        b->front_array = NULL;
    }
    if (b->back_array) {
        for (unsigned r = 0; r < b->rows; r++) {
            free(b->back_array[r]);
        }
        free(b->back_array);
        b->back_array = NULL;
    }

    b->rows = 0;
    b->columns = 0;
}

bool board_calloc_arrays(struct Board *b, unsigned rows, unsigned columns) {
    b->front_array = calloc(rows, sizeof(unsigned *));
    if (!b->front_array) {
        fprintf(stderr, "Error in calloc of front array rows!\n");
        return true;
    }

    for (unsigned r = 0; r < rows; r++)  {
        b->front_array[r] = calloc(columns, sizeof(unsigned));
        if (!b->front_array[r]) {
            fprintf(stderr, "Error in calloc of front array columns!\n");
            return true;
        }
    }

    b->back_array = calloc(rows, sizeof(unsigned *));
    if (!b->back_array) {
        fprintf(stderr, "Error in calloc of back array rows!\n");
        return true;
    }

    for (unsigned r = 0; r < rows; r++)  {
        b->back_array[r] = calloc(columns, sizeof(unsigned));
        if (!b->back_array[r]) {
            fprintf(stderr, "Error in calloc of back array columns!\n");
            return true;
        }
    }

    b->rows = rows;
    b->columns = columns;

    return false;
}

void board_set_theme(struct Board *b, unsigned theme) {
    b->theme = (theme - 1) * 16;
}

bool board_reset(struct Board *b, unsigned rows, unsigned columns, int scale, int mines) {
    board_free_arrays(b);

    if (board_calloc_arrays(b, rows, columns)) {
        return true;
    }

    for (unsigned r = 0; r < rows; r++)  {
        for (unsigned c = 0; c < columns; c++) {
            b->front_array[r][c] = 9;
            b->back_array[r][c] = 0;
        }
    }

    while (mines > 0) {
        int r = rand() % (int)rows;
        int c = rand() % (int)columns;
        if (!b->back_array[r][c]) {
            b->back_array[r][c] = 13;
            mines--;
        }
    }

    for (int row = 0; row < (int)rows; row++) {
        for (int column = 0; column < (int)columns; column++) {
            unsigned mine_count = 0;
            if (b->back_array[row][column] == 13) {
                continue;
            }
            for (int r = row - 1; r < row + 2; r++) {
                if (r < 0 || r >= (int)rows) {
                    continue;
                }
                for (int c = column - 1; c < column + 2; c++) {
                    if ( c >= 0 && c < (int)columns) {
                        if (b->back_array[r][c] == 13) {
                            mine_count++;
                        }
                    }
                }
            }
            b->back_array[row][column] = mine_count; 
        }
    }

    b->offset_x = (PIECE_SIZE - LEFT_OFFSET) * scale;
    b->offset_y = BORDER_SIZE * scale;
    b->image_rect.w = PIECE_SIZE * scale;
    b->image_rect.h = PIECE_SIZE * scale;
    b->board_rect.x = b->offset_x;
    b->board_rect.y = b->offset_y;
    b->board_rect.w = (int)b->columns * PIECE_SIZE * scale;
    b->board_rect.h = (int)b->rows * PIECE_SIZE * scale;
    return false;
}

bool board_mouse_down(struct Board *b, int x, int y, Uint8 button) {
    b->pressed = false;

    if (x < b->board_rect.x || x > b->board_rect.x + b->board_rect.w) {
        return false;
    }
    if (y < b->board_rect.y || y > b->board_rect.y + b->board_rect.h) {
        return false;
    }

    int row = (int)(( y - b->board_rect.y ) / (double)b->image_rect.h);
    int column = (int)(( x - b->board_rect.x ) / (double)b->image_rect.w);

    if (button == SDL_BUTTON_LEFT) {
        if (b->front_array[row][column] == 9) {
            b->pressed = true;
            return true;
        }
    } else if (button == SDL_BUTTON_RIGHT) {
        if (b->front_array[row][column] > 8 && b->front_array[row][column] < 12) {
            b->pressed = true;
            return true;
        }
    }

    return false;
}

bool board_mouse_up(struct Board *b, int x, int y, Uint8 button, int *game_status, int *mines_mark) {
    if (x < b->board_rect.x || x > b->board_rect.x + b->board_rect.w) {
        return false;
    }
    if (y < b->board_rect.y || y > b->board_rect.y + b->board_rect.h) {
        return false;
    }
    if (!b->pressed) return false;

    int row = (int)(( y - b->board_rect.y ) / (double)b->image_rect.h);
    int column = (int)(( x - b->board_rect.x ) / (double)b->image_rect.w);

    if (button == SDL_BUTTON_LEFT) {
        if (b->front_array[row][column] == 9) {
            if (b->back_array[row][column] == 13) {
                *game_status = -1;
            }
            b->front_array[row][column] = b->back_array[row][column];
            if (b->front_array[row][column] == 0) {
                if (board_push_check(b, row, column)) return true;
                board_run_check(b);
            }
            if (board_game_won(b)) {
                *game_status = 1;
            }
            return false;
        }
    }

    if (button == SDL_BUTTON_RIGHT) {
        if (b->front_array[row][column] == 9) {
            b->front_array[row][column]++;
            *mines_mark = -1;
        } else if (b->front_array[row][column] == 10) {
            b->front_array[row][column]++;
            *mines_mark = 1;
        } else if (b->front_array[row][column] == 11){
            b->front_array[row][column] = 9;
        }
    }

    return false;
}

bool board_push_check(struct Board *b, int row, int column) {
    struct Node *node = calloc(1, sizeof(struct Node));
    if (!node) return true;

    node->row = row;
    node->column = column;

    if (b->check_head) {
        node->next = b->check_head;
        b->check_head->previous = node;
    }
    b->check_head = node;

    return false;
}

void board_remove_check(struct Board *b, struct Node *node) {
    if (node->next) {
        node->next->previous = node->previous;
    }
    if (node->previous) {
        node->previous->next = node->next;
    } else {
        b->check_head = node->next;
    }

    free(node);
}

void board_free_check(struct Board *b) {
    while (b->check_head) {
        board_remove_check(b, b->check_head);
    }
}

void board_run_check(struct Board *b) {
    while (b->check_head) {
        struct Node *node = b->check_head;
        int row = node->row;
        int column = node->column;
        for (int r = row - 1; r < row + 2; r++) {
            if (r < 0 || r >= (int)b->rows) {
                continue;
            }
            for (int c = column - 1; c < column + 2; c++) {
                if ( c < 0 || c >= (int)b->columns) {
                    continue;
                }
                if (b->front_array[r][c] == 9) {
                    b->front_array[r][c] = b->back_array[r][c];
                    if (b->front_array[r][c] == 0 ) {
                        board_push_check(b, r, c);
                    }
                }
            }
        }
        board_remove_check(b, node);
    }
}

bool board_game_won(struct Board *b) {
    for (unsigned row = 0; row < b->rows; row++) {
        for (unsigned column = 0; column < b->columns; column++) {
            if (b->back_array[row][column] != 13) {
                if (b->front_array[row][column] != b->back_array[row][column]) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool board_draw(struct Board *b) {
    for (unsigned r = 0; r < b->rows; r++) {
        b->image_rect.y = (int)r * b->image_rect.h + b->offset_y;
        for (unsigned c = 0; c < b->columns; c++) {
            b->image_rect.x = (int)c * b->image_rect.w + b->offset_x;
            unsigned image_index = b->front_array[r][c];
            if (b->image_array[image_index + b->theme]) {
                if (SDL_RenderCopy(b->renderer, b->image_array[image_index + b->theme], NULL, &b->image_rect)) {
                    fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
                    return true;
                }
            }
        }
    }

    return false;
}
