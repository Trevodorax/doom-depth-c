#ifndef DOOM_DEPTH_C_FIGHT_MENU_H
#define DOOM_DEPTH_C_FIGHT_MENU_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../fight_screen.h"

typedef struct menu_t menu_t;
struct menu_t {
    // items contained by this menu
    int nb_options;
    menu_t ** options;

    // displayed elements (can be either 1 or 2 of those, but not none)
    char * title;
    char * image_path;

    fight_action action;
};

/**
 * @brief Menu for the fight screen
 *
 * Recursive function that displays the menu, and gets user input for each level of menu, returning the action when the
 * user gets to one.
 *
 * @param game_window
 * @param menu The displayed menu
 * @param fight_context Pointer to the data bout the fight
 * @param fight_zone Pointer to the rectangle where the fight is displayed
 * @param menu_zone Pointer to the rectangle where the menu is displayed
 * @param is_nested If the menu is called from another menu
 * @sideeffects modifies the fight and menu zone if window is resized
 * @return The action selected in the menu
 */
fight_action fight_menu(game_window_t * game_window, menu_t * menu, fight_context_t * fight_context, SDL_Rect * fight_zone, SDL_Rect * menu_zone, bool is_nested);

/**
 * @brief Prints one level of menu
 *
 * @param renderer
 * @param menu The printed menu
 * @param container The rectangle in which to print the menu
 * @param selected_item_index The index of the selected item
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_menu(SDL_Renderer * renderer, menu_t * menu, SDL_Rect * container, int selected_item_index);

/**
 * @brief Prints one item of one level of menu
 *
 * @param renderer
 * @param title Optional title (only need title or image or both)
 * @param image_path Optional image (only need title or image or both)
 * @param container The rectangle in which to print the menu item
 * @param is_selected If the user is currently selecting this menu item
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_menu_item(SDL_Renderer * renderer, const char * title, const char * image_path, SDL_Rect * container, bool is_selected);

/**
 * @brief Recursively frees a menu
 *
 * @param menu The freed menu
 */
void free_menu(menu_t * menu);

/**
 * @brief Gets a movement and returns the newly selected item
 *
 * @param selected_item_index The currently selected item
 * @param nb_items The total number of items
 * @param direction Which way the selection is moved
 * @return The new index or the previous one if the move is impossible
 */
int handle_fight_menu_movement(int selected_item_index, int nb_items, orientation_t direction);

menu_t* build_nested_menu();

#endif //DOOM_DEPTH_C_FIGHT_MENU_H
