#include "inventory_screen.h"
#include "../sdl_utils/sdl_utils.h"
#include "../utils/router.h"
#include "math.h"

int display_inventory(game_window_t *game_window, section_options active_section, category_options active_category, action_options active_action);

int inventory_screen(game_window_t *game_window) {
    SDL_Event e;
    bool quit = false;
    unsigned short active_section = CATEGORY;
    unsigned short active_category = WEAPONS;
    unsigned short active_action = USE;
    while (!quit){
        // TODO: link to next screens
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                return QUIT;
            }
            if (e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_DOWN && active_section == CATEGORY){
                    if (active_category != MANA_POTIONS) {
                        active_category++;
                    }
                }
                if (e.key.keysym.sym == SDLK_UP && active_section == CATEGORY){
                    if (active_category != GO_BACK) {
                        active_category--;
                    }
                }
            }
        }
        display_inventory(game_window, active_section, active_category, active_action);
    }
    return EXIT_SUCCESS;
}

int display_inventory(game_window_t *game_window, section_options active_section, category_options active_category, action_options active_action) {
    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    SDL_RenderClear(game_window->renderer);

    draw_fill_rect(
            (SDL_Rect){0, 0, window_width, window_height},
            (SDL_Color){0, 0, 0, 255},
            game_window->renderer
    );

    int unit = min(window_width, window_height) / 3;
    int unit_padding = unit / 10;

    int category_size = unit / 4;

    const int categories_count = 5;
    SDL_Rect categories[5];
    for (int i = 0; i < categories_count; i++) {
        categories[i] = (SDL_Rect) {
            unit_padding,
            i * category_size + unit_padding * (2*i + 1),
            category_size,
            category_size
        };
    }

    draw_image_in_rectangle(game_window->renderer, categories[GO_BACK], "../assets/inventory_categories/go_back.png", NORTH);
    draw_image_in_rectangle(game_window->renderer, categories[WEAPONS], "../assets/inventory_categories/weapons.png", NORTH);
    draw_image_in_rectangle(game_window->renderer, categories[ARMORS], "../assets/inventory_categories/armors.png", NORTH);
    draw_image_in_rectangle(game_window->renderer, categories[HEALTH_POTIONS], "../assets/inventory_categories/health_potions.png", NORTH);
    draw_image_in_rectangle(game_window->renderer, categories[MANA_POTIONS], "../assets/inventory_categories/mana_potions.png", NORTH);

    if (active_section == CATEGORY) {
        draw_thick_rect(categories[active_category], 2, (SDL_Color) {255, 255, 255, 255}, game_window->renderer);
        SDL_RenderPresent(game_window->renderer);
        return EXIT_SUCCESS;
    } else {
        draw_thick_rect(categories[active_category], 2, (SDL_Color) {255, 0, 0, 255}, game_window->renderer);
    }

    SDL_RenderPresent(game_window->renderer);

    return EXIT_SUCCESS;
}