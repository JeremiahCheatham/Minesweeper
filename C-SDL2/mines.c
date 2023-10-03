
#include "mines.h"
#include "load_media.h"

bool mines_new(struct Mines **mines, SDL_Renderer *renderer) {
    *mines = calloc(1, sizeof(struct Mines));
    if (!*mines) {
        fprintf(stderr, "Error in calloc of new mines.\n");
        return true;
    }
    struct Mines *m = *mines;

    m->renderer = renderer;

   load_media_sheet(
        m->renderer,
        &m->background_array,
        &m->background_length,
        &m->background_rect,
        "images/digitback.png",
        DIGIT_BACK_HEIGHT,
        DIGIT_BACK_WIDTH
    );

    load_media_sheet(
        m->renderer,
        &m->digits_array,
        &m->digits_length,
        &m->digits_rect,
        "images/digits.png",
        DIGIT_HEIGHT,
        DIGIT_WIDTH
    );

    m->background_rect.x = MINES_LEFT_OFFSET;
    m->background_rect.y = MINES_TOP_OFFSET;
    return false;
}

void mines_free(struct Mines **mines) {
    if (*mines) {
        if ((*mines)->background_array) {
            for (unsigned int i = 0; i < (*mines)->background_length; i++) {
                if ((*mines)->background_array[i]) {
                    SDL_DestroyTexture((*mines)->background_array[i]);
                }
            }
            free((*mines)->background_array);
        }

        if ((*mines)->digits_array) {
            for (unsigned int i = 0; i < (*mines)->digits_length; i++) {
                if ((*mines)->digits_array[i]) {
                    SDL_DestroyTexture((*mines)->digits_array[i]);
                }
            }
            free((*mines)->digits_array);
        }

        (*mines)->renderer = NULL;
        free(*mines);
        *mines = NULL;
    }
}

void mines_set_theme(struct Mines *m, int theme) {
    m->digit_theme = (theme - 1) * 12;
    m->back_theme = (theme - 1);
}

void mines_reset(struct Mines *m, int mine_count, int scale) {
    m->background_rect.x = MINES_LEFT_OFFSET * scale;
    m->background_rect.y = MINES_TOP_OFFSET * scale;
    m->background_rect.w = DIGIT_BACK_WIDTH * scale;
    m->background_rect.h = DIGIT_BACK_HEIGHT * scale;
    m->digits_rect.x = (MINES_LEFT_OFFSET + 1) * scale;
    m->digits_rect.y = (MINES_TOP_OFFSET + 1) * scale;
    m->digits_rect.w = DIGIT_WIDTH * scale;
    m->digits_rect.h = DIGIT_HEIGHT * scale;
    m->digit_x = m->digits_rect.x;
    m->mine_count = mine_count;
    mines_digits(m);
}

void mines_increment(struct Mines *m) {
    m->mine_count++;
    mines_digits(m);
}

void mines_decrement(struct Mines *m) {
    m->mine_count--;
    mines_digits(m);
}

void mines_digits(struct Mines *m) {
    int default_digit = 11;
    int mine_count = abs(m->mine_count);

    if (m->mine_count > 999) {
        mine_count = 999;
    } if (m->mine_count < -99) {
        mine_count = 99;
    }
   
    if (mine_count > 99) {
        m->digits[0] = (int)(mine_count / 100.0);
        default_digit = 0;
    } else {
        m->digits[0] = default_digit;
    }

    if (mine_count > 9) {
        m->digits[1] = (int)(mine_count / 10.0) % 10;
        default_digit = 0;
    } else {
        m->digits[1] = default_digit;
    }

    m->digits[2] = mine_count % 10;

    if (m->mine_count < 0) {
        m->digits[0] = 10;
    }
}

bool mines_draw(struct Mines *m) {
    if (SDL_RenderCopy(m->renderer, m->background_array[m->back_theme], NULL, &m->background_rect)) {
        fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
        return true;
    }

    for (int i = 0; i < 3; i++) {
        m->digits_rect.x = m->digit_x + m->digits_rect.w * i;
        if (SDL_RenderCopy(m->renderer, m->digits_array[m->digits[i] + m->digit_theme], NULL, &m->digits_rect)) {
            fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
            return true;
        }
    }

    return false;
}
