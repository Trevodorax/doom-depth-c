#include "fight_menu.h"
#include <stdlib.h>
#include <stdio.h>
#include "../fight_actions/fight_actions.h"
#include "../../sdl_utils/sdl_utils.h"

menu_t* create_menu(int nb_options, const char * title, const char * image_path, fight_action action) {
    menu_t *new_menu = malloc(sizeof(menu_t));
    new_menu->nb_options = nb_options;
    new_menu->options = malloc(sizeof(menu_t *) * nb_options);
    new_menu->title = title;
    new_menu->image_path = image_path;
    new_menu->action = action;
    return new_menu;
}

menu_t* build_nested_menu() {
    menu_t *root_menu = create_menu(4, "Root Menu", NULL, NULL);

    menu_t *submenu1 = create_menu(2, "Submenu1", NULL, NULL);
    submenu1->options[0] = create_menu(0, "Option1", NULL, action_1);
    submenu1->options[1] = create_menu(0, "Option2", NULL, action_2);

    menu_t *submenu2 = create_menu(1, "Submenu2", NULL, NULL);
    submenu2->options[0] = create_menu(0, "Option3", NULL, action_3);

    root_menu->options[0] = submenu1;
    root_menu->options[1] = submenu2;
    root_menu->options[2] = create_menu(0, "Option1", NULL, action_1);
    root_menu->options[3] = create_menu(0, "Option2", NULL, action_2);
    return root_menu;
}

void free_menu(menu_t * menu) {
    if (!menu)
        return;

    for (int i = 0; i < menu->nb_options; i++) {
        free_menu(menu->options[i]);
    }

    free(menu->title);
    free(menu->image_path);
    free(menu->options);
    free(menu);
}

int display_menu(SDL_Renderer * renderer, menu_t * menu, SDL_Rect * container, int selected_item_index) {
    if (!renderer || !menu || !container || selected_item_index >= menu->nb_options || selected_item_index < 0) {
        fprintf(stderr, "\ndisplay_menu error: Please provide all necessary arguments.");
        return EXIT_FAILURE;
    }

    SDL_Rect * menu_items_grid = get_rectangle_grid(menu->nb_options, container);
    for (int i = 0; i < menu->nb_options; i++) {
        if (display_menu_item(renderer, menu->options[i]->title, menu->options[i]->image_path, &menu_items_grid[i], i == selected_item_index) == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }
    free(menu_items_grid);

    return EXIT_SUCCESS;
}

int display_menu_item(SDL_Renderer * renderer, const char * title, const char * image_path, SDL_Rect * container, bool is_selected) {
    // safeguards
    if (!renderer || (!title && !image_path) || !container) {
        fprintf(stderr, "\ndisplay_menu_item error: please provide all necessary arguments.");
        return EXIT_FAILURE;
    }

    SDL_Rect item_container = *container;

    int selected_border_thickness = 5;

    if (is_selected) {
        item_container.x += selected_border_thickness;
        item_container.y += selected_border_thickness;
        item_container.w -= selected_border_thickness * 2;
        item_container.h -= selected_border_thickness * 2;
        draw_thick_rect(item_container, selected_border_thickness, (SDL_Color){40, 171, 112, 255}, renderer);
    }

    // draw the item background
    draw_fill_rect(item_container, (SDL_Color){66, 22, 144, 255}, renderer);

    // get the content zone
    int container_padding = item_container.w / 10;
    SDL_Rect content_container = {
            item_container.x + container_padding,
            item_container.y + container_padding,
            item_container.w - 2 * container_padding,
            item_container.h - 2 * container_padding
    };

    // print the image if there is one
    if(image_path) {
        SDL_Texture * image_texture = get_image_texture(renderer, image_path);
        if(!image_texture) {
            fprintf(stderr, "\ndisplay_menu_item error: could not retrieve image texture.");
            return EXIT_FAILURE;
        }

        SDL_RenderCopy(renderer, image_texture, NULL, &content_container);
    }

    // print the title if there is one
    if (title) {
        SDL_Texture *title_texture = get_string_texture(
                renderer,
                title,
                "../assets/PixelifySans-Bold.ttf",
                14,
                (SDL_Color) {255, 255, 255, 255}
        );
        if(!title_texture) {
            fprintf(stderr, "\ndisplay_menu_item error: could not retrieve title texture.");
            return EXIT_FAILURE;
        }

        // print the title texture in the middle of the container
        int title_texture_w, title_texture_h;
        SDL_QueryTexture(title_texture, NULL, NULL, &title_texture_w, &title_texture_h);
        int title_texture_x = content_container.x + (content_container.w - title_texture_w) / 2;
        int title_texture_y = content_container.y + (content_container.h - title_texture_h) / 2;
        SDL_Rect title_dst_rect = {title_texture_x, title_texture_y, title_texture_w, title_texture_h};
        SDL_RenderCopy(renderer, title_texture, NULL, &title_dst_rect);
        SDL_DestroyTexture(title_texture);
    }

    return EXIT_SUCCESS;
}

