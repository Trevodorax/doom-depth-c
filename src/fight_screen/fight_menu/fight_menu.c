#include "fight_menu.h"
#include <stdlib.h>
#include <stdio.h>
#include "../fight_actions/fight_actions.h"
#include "../../sdl_utils/sdl_utils.h"
#include "../fight_dimensions/fight_dimensions.h"

menu_t* create_menu(int nb_options, const char * title, const char * image_path, int (*callback)(fight_context_t *, void * custom_params), void * custom_params) {
    menu_t *new_menu = malloc(sizeof(menu_t));
    new_menu->nb_options = nb_options;
    new_menu->options = malloc(sizeof(menu_t *) * nb_options);
    new_menu->title = strdup(title);

    // no idea why this breaks, fuck C, this is a workaround
    if (image_path) {
        new_menu->image_path = malloc(strlen(image_path));
        strcpy(new_menu->image_path, image_path);
    } else {
        new_menu->image_path = malloc(1);
        new_menu->image_path[0] = '\0';
    }


    new_menu->action = malloc(sizeof(fight_action_t));
    new_menu->action->callback = callback;
    new_menu->action->params = custom_params;

    return new_menu;
}

menu_t* build_nested_menu(fight_context_t * fight_context) {
    int monsters_size = get_size(fight_context->monsters);

    menu_t *root_menu = create_menu(3, "Root Menu", NULL, NULL, NULL);

    menu_t *sub_menu_weapon = create_menu(monsters_size, "Weapon", NULL, NULL, NULL);
    menu_t *sub_menu_spells = create_menu(3, "Spells", NULL, NULL, NULL);

    sub_menu_spells->options[0] = create_menu(monsters_size, "Attack Spell", NULL, NULL, NULL);
    sub_menu_spells->options[1] = create_menu(0, "Defend Spell", NULL, defend_spell, NULL);
    sub_menu_spells->options[2] = create_menu(0, "Heal Spell", NULL, heal_spell, NULL);

    array_node_t * head = fight_context->monsters;
    int i = 0;
    while(head != NULL){
        monster_t * monster = (monster_t*)head->value;
        char * name_and_hp = malloc(sizeof(char)*(strlen(monster->name)+10));
        sprintf(name_and_hp,"%s (%d)",monster->name,monster->hp);
        sub_menu_weapon->options[i] = create_menu(0, name_and_hp, NULL, attack_weapon, monster);
        sub_menu_spells->options[0]->options[i++] = create_menu(0, name_and_hp, NULL, attack_spell, monster);
        head = head->next;
    }

    menu_t *sub_menu_others = create_menu(2, "Others", NULL, NULL, NULL);

    sub_menu_others->options[0] = create_menu(2, "Bag", NULL, NULL, NULL);
    sub_menu_others->options[0]->options[0] = create_menu(0, "Heal Potion", NULL, heal_potion, NULL);
    sub_menu_others->options[0]->options[1] = create_menu(0, "Mana Potion", NULL, mana_potion, NULL);

    sub_menu_others->options[1] = create_menu(0, "End Turn", NULL, end_turn, NULL);

    root_menu->options[0] = sub_menu_weapon;
    root_menu->options[1] = sub_menu_spells;
    root_menu->options[2] = sub_menu_others;
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

fight_action_t * fight_menu(game_window_t * game_window, menu_t * menu, fight_context_t * fight_context, SDL_Rect * fight_zone, SDL_Rect * menu_zone, bool is_nested) {
    int selected_item_index = 0;

    SDL_Event e;
    bool quit = false;
    while (!quit){
        SDL_Delay(50);
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym) {
                    case SDLK_z:
                        selected_item_index = handle_fight_menu_movement(selected_item_index, menu->nb_options, NORTH);
                        break;
                    case SDLK_d:
                        selected_item_index = handle_fight_menu_movement(selected_item_index, menu->nb_options, EAST);
                        break;
                    case SDLK_s:
                        selected_item_index = handle_fight_menu_movement(selected_item_index, menu->nb_options, SOUTH);
                        break;
                    case SDLK_q:
                        selected_item_index = handle_fight_menu_movement(selected_item_index, menu->nb_options, WEST);
                        break;
                    case SDLK_ESCAPE:
                        if (is_nested) {
                            return NULL;
                        }
                        break;
                    case SDLK_RETURN: {
                        if(!fight_context->player_turn){
                            break;
                        }
                        menu_t * clicked_menu = menu->options[selected_item_index];
                        if (clicked_menu->nb_options > 0) {
                            fight_action_t * selected_action = fight_menu(game_window, clicked_menu, fight_context, fight_zone, menu_zone, true);
                            if (!selected_action->callback) {
                                break;
                            } else {
                                return selected_action;
                            }
                        }
                        if (clicked_menu->action->callback) {
                            return clicked_menu->action;
                        }
                        // if there is no submenu AND no action, then there is a problem and nothing happens
                        return NULL;
                        break;
                    }
                    default:
                        break;
                }
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
        }
        update_section_dimensions(game_window->window, fight_zone, menu_zone);
        display_fight(game_window->renderer, fight_context, fight_zone);
        display_menu(game_window->renderer, menu, menu_zone, selected_item_index, fight_context->player_turn);
        SDL_RenderPresent(game_window->renderer);
    }

    return EXIT_SUCCESS;
}

int display_menu(SDL_Renderer * renderer, menu_t * menu, SDL_Rect * container, int selected_item_index, bool player_turn) {
    if (!renderer || !menu || !container || selected_item_index >= menu->nb_options || selected_item_index < 0) {
        fprintf(stderr, "\ndisplay_menu error: Please provide all necessary arguments.");
        return EXIT_FAILURE;
    }

    int a_value = player_turn ? 255 : 80;
    draw_fill_rect(*container, (SDL_Color){0, 0, 0, a_value}, renderer);

    SDL_Rect * menu_items_grid = get_rectangle_grid(menu->nb_options, container);
    for (int i = 0; i < menu->nb_options; i++) {
        if (display_menu_item(renderer, menu->options[i]->title, menu->options[i]->image_path, &menu_items_grid[i], i == selected_item_index) == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }
    free(menu_items_grid);

    return EXIT_SUCCESS;
}

int display_fight(SDL_Renderer * renderer, fight_context_t * fight_context, SDL_Rect * fight_zone) {
    if (!renderer) {
        fprintf(stderr, "\ndisplay_menu error: Please provide all necessary arguments.");
        return EXIT_FAILURE;
    }

    draw_fill_rect(*fight_zone, (SDL_Color){0, 180, 50, 255}, renderer);


    // TODO add notification part
    //  add fight_context->notification_message in top of the screen

    // TODO add fight part

    return EXIT_SUCCESS;
}

int display_menu_item(SDL_Renderer * renderer, const char * title, const char * image_path, SDL_Rect * container, bool is_selected) {

    int a_value = is_selected ? 255 : 80;
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
    draw_fill_rect(item_container, (SDL_Color){66, 22, 144, a_value}, renderer);

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
        SDL_Texture * image_texture = get_image_texture(renderer, image_path);
        if (!image_texture) {
            fprintf(stderr, "\ndisplay_menu_item error: could not retrieve image texture.");
            return EXIT_FAILURE;
        }

        SDL_RenderCopy(renderer, image_texture, NULL, &content_container);
        SDL_DestroyTexture(image_texture);
    }

    // print the title if there is one
    if (title) {
        SDL_Texture *title_texture = get_string_texture(
                renderer,
                title,
                "../assets/PixelifySans-Bold.ttf",
                14,
                (SDL_Color) {255, 255, 255, a_value >= 235 ? 255 : a_value+20}
        );
        if (!title_texture) {
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

int handle_fight_menu_movement(int selected_item_index, int nb_items, orientation_t direction) {
    int nb_rows = (int)sqrt(nb_items);
    int nb_cols = (nb_items + nb_rows - 1) / nb_rows;

    switch (direction) {
        case NORTH:
            selected_item_index -= nb_cols;
            break;
        case EAST:
            selected_item_index ++;
            break;
        case SOUTH:
            selected_item_index += nb_cols;
            break;
        case WEST:
            selected_item_index--;
            break;
        default:
            return selected_item_index;
    }

    if (selected_item_index < 0) {
        return 0;
    }

    if (selected_item_index >= nb_items) {
        return nb_items - 1;
    }

    return selected_item_index;
}
