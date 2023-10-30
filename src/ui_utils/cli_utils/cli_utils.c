#include "./cli_utils.h"

SDL_Rect rect_to_SDL_Rect(rect_t rect) {
    return (SDL_Rect) {
            (int)rect.x,
            (int)rect.y,
            (int)rect.w,
            (int)rect.h
    };
}
