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
 * @param renderer
 * @param fight_context
 * @param container The rectangle in which to display the menu
 * @return The action selected in the menu
 */
fight_action fight_menu(SDL_Renderer * renderer, fight_context_t * fight_context, SDL_Rect * container);

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

menu_t* build_nested_menu();

#endif //DOOM_DEPTH_C_FIGHT_MENU_H
