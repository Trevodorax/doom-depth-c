#include "event.h"

event_t cli_get_event();
event_t sdl_get_event();
event_t char_to_event(char c);


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

    return char_to_event(event);
}

char event_to_char(event_t event) {
    if (event >= A_KEY && event <= Z_KEY) {
        return 'A' + (event - A_KEY);
    } else if (event >= a_KEY && event <= z_KEY) {
        return 'a' + (event - a_KEY);
    } else {
        switch (event) {
            case SPACE_KEY: return ' ';
            case LPAREN_KEY: return '(';
            case RPAREN_KEY: return ')';
            case GT_KEY: return '>';
            case ENTER_KEY: return '\n';
            case ESCAPE_KEY: return (char)27;
            default: return '?';
        }
    }
}

event_t char_to_event(char c) {
    if (isupper(c)) {
        return (event_t)(A_KEY + (c - 'A'));
    } else if (islower(c)) {
        return (event_t)(a_KEY + (c - 'a'));
    } else {
        switch (c) {
            case ' ': return SPACE_KEY;
            case '(': return LPAREN_KEY;
            case ')': return RPAREN_KEY;
            case '>': return GT_KEY;
            case '\n': return ENTER_KEY;
            case 27: return ESCAPE_KEY;
            default: return UNKNOWN_EVENT;
        }
    }
}

event_t sdl_get_event() {
    // get event
    SDL_Event event;
    if (!SDL_PollEvent(&event)) {
        return UNKNOWN_EVENT;
    }

    // parse event
    if (event.type == SDL_QUIT) {
        return QUIT;
    }
    if (event.type == SDL_KEYDOWN) {
        int shiftPressed = event.key.keysym.mod & KMOD_SHIFT;
        int capsLockOn = event.key.keysym.mod & KMOD_CAPS;

        // uppercase letters
        if (event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z && (shiftPressed || capsLockOn)) {
            return (event_t)(A_KEY + (event.key.keysym.sym - SDLK_a));
        }
        // lowercase letters
        if (event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z && !(shiftPressed || capsLockOn)) {
            return (event_t)(a_KEY + (event.key.keysym.sym - SDLK_a));
        }

        switch (event.key.keysym.sym) {
            case SDLK_SPACE: return SPACE_KEY;
            case SDLK_LEFTPAREN: return LPAREN_KEY;
            case SDLK_RIGHTPAREN: return RPAREN_KEY;
            case SDLK_GREATER: return GT_KEY;
            case SDLK_RETURN:
            case SDLK_KP_ENTER: return ENTER_KEY;
            case SDLK_ESCAPE: return ESCAPE_KEY;
            default: return UNKNOWN_EVENT;
        }
    }

    return UNKNOWN_EVENT;
}
