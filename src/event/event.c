#include "event.h"

event_t cli_get_event();
event_t sdl_get_event();

int get_event(ui_type_t ui_type, event_t * event) {
    switch (ui_type) {
        case CLI:
            *event = cli_get_event();
            break;
        case GUI:
            *event = sdl_get_event();
            break;
    }

    return *event != -1;
}

event_t cli_get_event() {
    char event;
    if (!cli_poll_char(&event)) {
        return -1;
    }

    size_t nb_keys_in_map = sizeof(event_to_char_map) / sizeof(event_to_char_map[0]);

    for (size_t i = 0; i < nb_keys_in_map; i++) {
        if(event_to_char_map[i] == event) {
            return i;
        }
    }

    return UNKNOWN_EVENT;
}

event_t sdl_get_event() {
    // get event
    SDL_Event event;
    if (!SDL_PollEvent(&event)) {
        return -1;
    }

    // parse event
    if (event.type == SDL_QUIT){
        return QUIT;
    }
    if (event.type == SDL_KEYDOWN){
        switch (event.key.keysym.sym) {
            case SDLK_z:
                return Z_KEY;
            case SDLK_d:
                return D_KEY;
            case SDLK_s:
                return S_KEY;
            case SDLK_q:
                if (event.key.keysym.mod & KMOD_SHIFT) { // uppercase check
                    return QUIT;
                } else {
                    return Q_KEY;
                }
            default:
                return Z_KEY;
        }
    }

    return UNKNOWN_EVENT;
}
