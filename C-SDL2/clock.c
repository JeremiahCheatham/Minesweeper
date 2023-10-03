#include "clock.h"
#include "load_media.h"

bool clock_new(struct Clock **clock, SDL_Renderer *renderer) {
    *clock = calloc(1, sizeof(struct Clock));
    if (!*clock) {
        fprintf(stderr, "Error in calloc of new clock.\n");
        return true;
    }
    struct Clock *c = *clock;

    c->renderer = renderer;

   load_media_sheet(
        c->renderer,
        &c->background_array,
        &c->background_length,
        &c->background_rect,
        "images/digitback.png",
        DIGIT_BACK_HEIGHT,
        DIGIT_BACK_WIDTH
    );

    load_media_sheet(
        c->renderer,
        &c->digits_array,
        &c->digits_length,
        &c->digits_rect,
        "images/digits.png",
        DIGIT_HEIGHT,
        DIGIT_WIDTH
    );

    return false;
}

void clock_free(struct Clock **clock) {
    if (*clock) {
        if ((*clock)->background_array) {
            for (unsigned int i = 0; i < (*clock)->background_length; i++) {
                if ((*clock)->background_array[i]) {
                    SDL_DestroyTexture((*clock)->background_array[i]);
                }
            }
            free((*clock)->background_array);
        }

        if ((*clock)->digits_array) {
            for (unsigned int i = 0; i < (*clock)->digits_length; i++) {
                if ((*clock)->digits_array[i]) {
                    SDL_DestroyTexture((*clock)->digits_array[i]);
                }
            }
            free((*clock)->digits_array);
        }

        (*clock)->renderer = NULL;
        free(*clock);
        *clock = NULL;
    }
}

void clock_set_theme(struct Clock *c, int theme) {
    c->digit_theme = (theme - 1) * 12;
    c->back_theme = (theme - 1);
}

void clock_reset(struct Clock *c, int window_width, int scale) {
    c->background_rect.x = window_width - (DIGIT_BACK_WIDTH + CLOCK_RIGHT_OFFSET) * scale;
    c->background_rect.y = MINES_TOP_OFFSET * scale;
    c->background_rect.w = DIGIT_BACK_WIDTH * scale;
    c->background_rect.h = DIGIT_BACK_HEIGHT * scale;
    c->digits_rect.x = c->background_rect.x + scale;
    c->digits_rect.y = c->background_rect.y + scale;
    c->digits_rect.w = DIGIT_WIDTH * scale;
    c->digits_rect.h = DIGIT_HEIGHT * scale;
    c->digit_x = c->digits_rect.x;
    c->last_time = SDL_GetTicks();
    c->seconds = 0;
    clock_digits(c);
}

void clock_digits(struct Clock *c) {
    int default_digit = 11;
    int seconds = abs(c->seconds);

    if (c->seconds > 999) {
        seconds = 999;
    }
   
    if (seconds > 99) {
        c->digits[0] = (int)(seconds / 100.0);
        default_digit = 0;
    } else {
        c->digits[0] = default_digit;
    }

    if (seconds > 9) {
        c->digits[1] = (int)(seconds / 10.0) % 10;
        default_digit = 0;
    } else {
        c->digits[1] = default_digit;
    }

    c->digits[2] = seconds % 10;
}

void clock_update(struct Clock *c) {
    Uint32 current_time = SDL_GetTicks();
    Uint32 elapsed_time = 0;

    if (current_time >= c->last_time) {
        elapsed_time = current_time - c->last_time;
    } else {
        elapsed_time = (Uint32)-1 - c->last_time + current_time;
    }

    if (elapsed_time > 1000) {
        c->last_time += 1000;
        c->seconds++;
        clock_digits(c);
    }
}

bool clock_draw(struct Clock *c) {
    if (SDL_RenderCopy(c->renderer, c->background_array[c->back_theme], NULL, &c->background_rect)) {
        fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
        return true;
    }

    for (int i = 0; i < 3; i++) {
        c->digits_rect.x = c->digit_x + c->digits_rect.w * i;
        if (SDL_RenderCopy(c->renderer, c->digits_array[c->digits[i] + c->digit_theme], NULL, &c->digits_rect)) {
            fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
            return true;
        }
    }

    return false;
}
