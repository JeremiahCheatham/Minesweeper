#include "border.h"
#include "load_media.h"

bool border_new(struct Border **border, SDL_Renderer *renderer) {
    *border = calloc(1, sizeof(struct Border));
    if (!*border) {
        fprintf(stderr, "Error in calloc of new border.\n");
        return true;
    }
    struct Border *b = *border;

    b->renderer = renderer;

   load_media_sheet(
        b->renderer,
        &b->image_array,
        &b->image_length,
        &b->rect,
        "images/borders.png",
        BORDER_SIZE,
        PIECE_SIZE
    );

    return false;
}

void border_free(struct Border **border) {
    if (*border) {
        load_media_free(&(*border)->image_array, (*border)->image_length);

        (*border)->renderer = NULL;
        free(*border);
        *border = NULL;
    }
}

void border_set_theme(struct Border *b, int theme) {
    b->theme = (theme - 1) * 8;
}

void border_reset(struct Border *b, int rows, int columns, int scale) {
    b->rows = rows;
    b->columns = columns;
    b->offset_x = LEFT_OFFSET * scale;
    b->piece_size = PIECE_SIZE * scale;
    b->border_size = BORDER_SIZE * scale;
    b->rect.w = b->piece_size;
    b->rect.h = b->border_size;
}

bool border_draw(struct Border *b) {
    b->rect.x = -b->offset_x;
    b->rect.y = 0;
    if (SDL_RenderCopy(b->renderer, b->image_array[0 + b->theme], NULL, &b->rect)) {
        fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
        return true;
    }

    b->rect.x = b->piece_size * (b->columns + 1) - b->offset_x;
    b->rect.y = 0;
    if (SDL_RenderCopy(b->renderer, b->image_array[2 + b->theme], NULL, &b->rect)) {
        fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
        return true;
    }

    b->rect.x = -b->offset_x;
    b->rect.y = b->piece_size * b->rows + b->border_size;
    if (SDL_RenderCopy(b->renderer, b->image_array[5 + b->theme], NULL, &b->rect)) {
        fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
        return true;
    }

    b->rect.x = b->piece_size * (b->columns + 1) - b->offset_x;
    b->rect.y = b->piece_size * b->rows + b->border_size;
    if (SDL_RenderCopy(b->renderer, b->image_array[7 + b->theme], NULL, &b->rect)) {
        fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
        return true;
    }

    for (int r = 0; r < b->rows; r++) {
        b->rect.x = -b->offset_x;
        b->rect.y = r * b->piece_size + b->border_size;
        if (SDL_RenderCopy(b->renderer, b->image_array[3 + b->theme], NULL, &b->rect)) {
            fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
            return true;
        }

        b->rect.x = b->piece_size * (b->columns + 1) - b->offset_x;
        b->rect.y = r * b->piece_size + b->border_size;
        if (SDL_RenderCopy(b->renderer, b->image_array[4 + b->theme], NULL, &b->rect)) {
            fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
            return true;
        }
    }

    for (int c = 0; c < b->columns; c++) {
        b->rect.x = (c + 1) * b->piece_size - b->offset_x;
        b->rect.y = 0;
        if (SDL_RenderCopy(b->renderer, b->image_array[1 + b->theme], NULL, &b->rect)) {
            fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
            return true;
        }

        b->rect.x = (c + 1) * b->piece_size - b->offset_x;
        b->rect.y = b->piece_size * b->rows + b->border_size;
        if (SDL_RenderCopy(b->renderer, b->image_array[6 + b->theme], NULL, &b->rect)) {
            fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
            return true;
        }
    }

    return false;
}
