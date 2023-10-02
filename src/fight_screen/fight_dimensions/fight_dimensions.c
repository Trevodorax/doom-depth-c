#include "fight_dimensions.h"

void update_section_dimensions(SDL_Window * window, SDL_Rect * fight_zone, SDL_Rect * menu_zone) {
    // get the dimensions of window
    int window_height = 0;
    int window_width = 0;
    SDL_GetWindowSize(window, &window_width, &window_height);

    // use them to define zones for different zones
    fight_zone->x = 0;
    fight_zone->y = 0;
    fight_zone->w = window_width;
    fight_zone->h = window_height / 2;

    menu_zone->x = 0;
    menu_zone->y = window_height / 2;
    menu_zone->w = window_width;
    menu_zone->h = window_height / 2;
}
