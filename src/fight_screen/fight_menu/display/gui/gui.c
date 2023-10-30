#include "gui.h"
#include "../../../../ui_utils/ui_utils.h"

int display_menu_item_gui(game_window_t * game_window, const char * title, const char * image_path, SDL_Rect container,
                          bool is_selected);

int display_menu_gui(game_window_t *game_window, menu_t * menu, rect_t container, int selected_item_index, bool player_turn) {
    if (!game_window || !menu || selected_item_index >= menu->nb_options || selected_item_index < 0) {
        global_logger->error("\ndisplay_menu error: Please provide all necessary arguments.");
        return EXIT_FAILURE;
    }

    int a_value = player_turn ? 255 : 80;
    draw_fill_rect(rect_to_SDL_Rect(container), (SDL_Color){0, 0, 0, a_value}, game_window->renderer);

    rect_t * menu_items_grid = get_rectangle_grid(menu->nb_options, &container);
    for (int i = 0; i < menu->nb_options; i++) {
        if (display_menu_item_gui(game_window, menu->options[i]->title, menu->options[i]->image_path, rect_to_SDL_Rect(menu_items_grid[i]),
                                  i == selected_item_index) == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }
    free(menu_items_grid);

    return EXIT_SUCCESS;
}

int display_menu_item_gui(game_window_t *game_window, const char * title, const char * image_path, SDL_Rect container,
                          bool is_selected) {
    // safeguards
    if (!game_window || !game_window->renderer || (!title && !image_path)) {
        global_logger->error("\ndisplay_menu_item error: please provide all necessary arguments.");
        return EXIT_FAILURE;
    }

    int a_value = is_selected ? 255 : 80;

    SDL_Rect item_container = container;

    int selected_border_thickness = 5;

    if (is_selected) {
        item_container.x += selected_border_thickness;
        item_container.y += selected_border_thickness;
        item_container.w -= selected_border_thickness * 2;
        item_container.h -= selected_border_thickness * 2;
        draw_thick_rect(
                item_container,
                selected_border_thickness,
                (SDL_Color){40, 171, 112, 255},
                game_window->renderer
        );
    }

    // draw the item background
    draw_fill_rect(item_container, (SDL_Color){66, 22, 144, a_value}, game_window->renderer);

    // get the content zone
    int container_padding = item_container.w / 10;
    SDL_Rect content_container = {
            item_container.x + container_padding,
            item_container.y + container_padding,
            item_container.w - 2 * container_padding,
            item_container.h - 2 * container_padding
    };

    // print the image if there is one
    if (strlen(image_path) > 0) {
        SDL_Texture * image_texture = get_image_texture(game_window->renderer, image_path);
        if (!image_texture) {
            global_logger->error("\ndisplay_menu_item error: could not retrieve image texture.");
            return EXIT_FAILURE;
        }

        SDL_RenderCopy(game_window->renderer, image_texture, NULL, &content_container);
        SDL_DestroyTexture(image_texture);
    }

    // print the title if there is one
    if (title) {
        SDL_Texture *title_texture = get_string_texture(
                game_window->renderer,
                title,
                "../assets/PixelifySans-Bold.ttf",
                14,
                (SDL_Color) {255, 255, 255, a_value >= 235 ? 255 : a_value+20}
        );
        if (!title_texture) {
            global_logger->error("\ndisplay_menu_item error: could not retrieve title texture.");
            return EXIT_FAILURE;
        }

        // print the title texture in the middle of the container
        int title_texture_w, title_texture_h;
        SDL_QueryTexture(title_texture, NULL, NULL, &title_texture_w, &title_texture_h);
        int title_texture_x = content_container.x + (content_container.w - title_texture_w) / 2;
        int title_texture_y = content_container.y + (content_container.h - title_texture_h) / 2;
        SDL_Rect title_dst_rect = {title_texture_x, title_texture_y, title_texture_w, title_texture_h};
        SDL_RenderCopy(game_window->renderer, title_texture, NULL, &title_dst_rect);
        SDL_DestroyTexture(title_texture);
    }

    return EXIT_SUCCESS;
}
