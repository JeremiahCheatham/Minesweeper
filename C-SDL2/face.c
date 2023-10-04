#include "face.h"
#include "load_media.h"

bool face_new(struct Face **face, SDL_Renderer *renderer) {
    *face = calloc(1, sizeof(struct Face));
    if (!*face) {
        fprintf(stderr, "Error in calloc of new face.\n");
        return true;
    }
    struct Face *f = *face;

    f->renderer = renderer;

   load_media_sheet(
        f->renderer,
        &f->image_array,
        &f->image_length,
        &f->image_rect,
        "images/faces.png",
        FACE_SIZE,
        FACE_SIZE
    );

    f->image_index = 0;

    return false;
}

void face_free(struct Face **face) {
    if (*face) {
        load_media_free(&(*face)->image_array, (*face)->image_length);

        (*face)->renderer = NULL;
        free(*face);
        *face = NULL;
    }
}

void face_set_theme(struct Face *f, int theme) {
    f->theme = (theme - 1) * 5;
}

void face_reset(struct Face *f, int window_width, int scale) {
    f->image_rect.x = (int)((window_width - (FACE_SIZE  * scale) + (LEFT_OFFSET * scale)) / 2.0);
    f->image_rect.y = FACE_TOP_OFFSET * scale;
    f->image_rect.w = FACE_SIZE * scale;
    f->image_rect.h = FACE_SIZE * scale;
}

bool face_mouse_click(struct Face *f, int x, int y, bool down) {
    if (x >= f->image_rect.x && x <= f->image_rect.x + f->image_rect.w) {
        if (y >= f->image_rect.y && y <= f->image_rect.y + f->image_rect.h) {
            if (down) {
                f->image_index = 1;
            } else if (f->image_index == 1) {
                f->image_index = 0;
                return true;
            }
        } 
    }

    return false;
}

void face_default(struct Face *f) {
    f->image_index = 0;
}

void face_won(struct Face *f) {
    f->image_index = 3;
}

void face_lost(struct Face *f) {
    f->image_index = 4;
}

void face_question(struct Face *f) {
    f->image_index = 2;
}

bool face_draw(struct Face *f) {
    if (SDL_RenderCopy(f->renderer, f->image_array[f->image_index + f->theme], NULL, &f->image_rect)) {
        fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
        return true;
    }

    return false;
}
