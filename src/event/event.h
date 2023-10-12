#ifndef DOOM_DEPTH_C_EVENT_H
#define DOOM_DEPTH_C_EVENT_H

#include "../game_window/game_window.h"

typedef enum {
    UNKNOWN_EVENT,
    QUIT,
    Z_KEY,
    Q_KEY,
    S_KEY,
    D_KEY
} event_t;

static char event_to_char_map[] = {
        -1,
        'Q',
        'z',
        'q',
        's',
        'd'
};

/**
 * @brief retrieves a ui type agnostic event
 *
 * @param ui_type The ui_type currently used
 * @param event The event to assign the value to
 *
 * @return true if there was an event, false otherwise
 */
int get_event(ui_type_t ui_type, event_t * event);

#endif //DOOM_DEPTH_C_EVENT_H
