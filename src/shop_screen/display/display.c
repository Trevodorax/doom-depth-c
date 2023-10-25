#include "display.h"
#include "../../utils/utils.h"
#include "../../utils/items_management/types.h"
#include "../../sdl_utils/sdl_utils.h"
#include "../utils/shop_utils.h"

int display_shop_gui(game_window_t *game_window, player_t *player,
                     section_options active_section, category_options active_category,
                     confirm_options active_confirmation, unsigned short active_item);
int display_go_back(SDL_Renderer *renderer, SDL_Rect *icon_container, SDL_Rect *text_container, int font_size);
int display_gold(SDL_Renderer *renderer, player_t *player, SDL_Rect *icon_container, SDL_Rect *text_container, int font_size);
int display_shop_categories(SDL_Renderer *renderer, SDL_Rect *categories_container,
                            SDL_Rect *go_back_icon_container, SDL_Rect *go_back_text_container,
                            category_options active_category, int font_size);

int display_shop_cli(game_window_t *game_window, player_t *player);

int display_shop(game_window_t *game_window, player_t *player,
                 section_options active_section, category_options active_category,
                 confirm_options active_confirmation, unsigned short active_item) {
    switch (game_window->ui_type) {
        case GUI:
            return display_shop_gui(game_window, player, active_section, active_category, active_confirmation, active_item);
        case CLI:
            return display_shop_cli(game_window, player);
    }
}

int display_shop_gui(game_window_t *game_window,
                     player_t *player,
                     section_options active_section,
                     category_options active_category,
                     confirm_options active_confirmation,
                     unsigned short active_item) {
    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    int unit = (min(window_width, window_height) == window_width) ? window_width / 4 : window_height / 5;
    int unit_padding = unit / 10;
    int icon_size = 2 * unit_padding;
    int font_size = icon_size;

    SDL_RenderClear(game_window->renderer);

    SDL_Rect window_rect = (SDL_Rect) {0, 0, window_width, window_height};

    // draw black background
    draw_fill_rect(window_rect, (SDL_Color) {0, 0, 0, 0}, game_window->renderer);

    SDL_Rect go_back_icon_rect = (SDL_Rect) {
        unit_padding,
        unit_padding,
        icon_size,
        icon_size,
    };

    SDL_Rect go_back_text_rect = (SDL_Rect) {
            unit_padding + go_back_icon_rect.w + unit_padding,
            unit_padding,
            0,
            0
    };

    SDL_Rect gold_icon_rect = (SDL_Rect) {
        window_width - unit_padding - icon_size,
        unit_padding,
        icon_size,
        icon_size
    };

    SDL_Rect gold_rect = (SDL_Rect) {
            gold_icon_rect.x - unit_padding,
            unit_padding,
            0,
            0
    };

    SDL_Rect items_container = (SDL_Rect) {
        0,
        window_height - 3 * unit - 2 * unit_padding,
        3 * unit - 2 * unit_padding,
        3 * unit - 2 * unit_padding
    };

    SDL_Rect categories_container = (SDL_Rect) {
        unit_padding,
        items_container.y - unit_padding / 2 - font_size,
        window_width - 2 * unit_padding,
        font_size
    };

    SDL_Rect merchant_image_rect = (SDL_Rect) {
        unit_padding,
        go_back_icon_rect.y + go_back_icon_rect.h + unit_padding,
        unit - unit_padding,
        items_container.x - (go_back_icon_rect.y + go_back_icon_rect.h + 2 * unit_padding)
    };

    SDL_Rect dialog_rect = (SDL_Rect) {
        merchant_image_rect.x + merchant_image_rect.w + unit_padding,
        merchant_image_rect.y,
        window_width - (merchant_image_rect.x + merchant_image_rect.w + 2 * unit_padding),
        merchant_image_rect.h
    };

    if (display_gold(game_window->renderer, player, &gold_icon_rect, &gold_rect, font_size)) {
        return EXIT_FAILURE;
    }
    if (display_shop_categories(game_window->renderer, &categories_container, &go_back_icon_rect, &go_back_text_rect, active_category, font_size)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int display_go_back(SDL_Renderer *renderer, SDL_Rect *icon_container, SDL_Rect *text_container, int font_size) {
    draw_image_in_rectangle(
            renderer,
            *icon_container,
            "../assets/items_mgmt/image/go_back.png",
            NORTH
    );

    SDL_Texture *go_back_text_texture = get_string_texture(
            renderer,
            "Go back",
            "../assets/PixelifySans-Regular.ttf",
            font_size,
            (SDL_Color) {255, 255, 255, 255}
    );
    if (!go_back_text_texture) {
        return EXIT_FAILURE;
    }

    SDL_QueryTexture(go_back_text_texture, NULL, NULL, &(text_container->w), &(text_container->h));

    SDL_RenderCopy(renderer, go_back_text_texture, NULL, text_container);
    SDL_DestroyTexture(go_back_text_texture);

    return EXIT_SUCCESS;
}

int display_gold(SDL_Renderer *renderer, player_t *player, SDL_Rect *icon_container, SDL_Rect *text_container, int font_size) {
    draw_image_in_rectangle(
            renderer,
            *icon_container,
            "../assets/items_mgmt/image/gold.png",
            NORTH
    );

    SDL_Texture *gold_texture = get_string_texture(
            renderer,
            player_gold_to_string(player),
            "../assets/PixelifySans-Regular.ttf",
            font_size,
            (SDL_Color) {255, 255, 255, 255}
    );
    if (!gold_texture) {
        return EXIT_FAILURE;
    }

    SDL_QueryTexture(gold_texture, NULL, NULL, &(text_container->w), &(text_container->h));
    text_container->x -= text_container->w;

    SDL_RenderCopy(renderer, gold_texture, NULL, text_container);
    SDL_DestroyTexture(gold_texture);

    return EXIT_SUCCESS;
}

int display_shop_categories(SDL_Renderer *renderer, SDL_Rect *categories_container,
                       SDL_Rect *go_back_icon_container, SDL_Rect *go_back_text_container,
                       category_options active_category, int font_size) {
    SDL_Color white = (SDL_Color) {255, 255, 255, 255};
    if (display_go_back(renderer, go_back_icon_container, go_back_text_container, font_size)) {
        return EXIT_FAILURE;
    }

    SDL_Rect weapons_container = (SDL_Rect) {
            categories_container->x,
            categories_container->y,
            categories_container->w / 3,
            categories_container->h
    };
    draw_string_in_rectangle(renderer, weapons_container, "WEAPONS", font_size, white);

    SDL_Rect armors_container = (SDL_Rect) {
            weapons_container.x + weapons_container.w,
            categories_container->y,
            categories_container->w / 3,
            categories_container->h
    };
    draw_string_in_rectangle(renderer, armors_container, "ARMORS", font_size, white);

    SDL_Rect potions_container = (SDL_Rect) {
            armors_container.x + armors_container.w,
            categories_container->y,
            categories_container->w / 3,
            categories_container->h
    };
    draw_string_in_rectangle(renderer, potions_container, "POTIONS", font_size, white);

    switch (active_category) {
        case GO_BACK:
            draw_thick_rect(*go_back_icon_container, 2, white, renderer);
            break;

        case ARMORS:
            draw_thick_rect(
                    (SDL_Rect) {armors_container.x, armors_container.y + armors_container.h + font_size / 2 - 4, armors_container.w, 2},
                    1, white, renderer);
            break;

        case WEAPONS:
            draw_thick_rect(
                    (SDL_Rect) {weapons_container.x, weapons_container.y + weapons_container.h + font_size / 2 - 4, weapons_container.w, 2},
                    1, white, renderer);
            break;

        case HEALTH_POTIONS:
        case MANA_POTIONS:
            draw_thick_rect(
                    (SDL_Rect) {potions_container.x, potions_container.y + potions_container.h + font_size / 2 - 4, potions_container.w, 2},
                    1, white, renderer);
            break;
    }

    return EXIT_SUCCESS;
}

int display_shop_cli(game_window_t *game_window, player_t *player){

}