#ifndef DOOM_DEPTH_C_INVENTORY_DISPLAY_H
#define DOOM_DEPTH_C_INVENTORY_DISPLAY_H

#include "../../game_window/game_window.h"
#include "../../entities/inventory/inventory.h"
#include "../inventory_screen.h"
#include "../../utils/items_management/types.h"

/**
 * @brief displays inventory depending on where the user is in the user experience, three sections are available :
 * categories, items and their details, actions
 *
 * @param game_window the window on which to display the inventory
 * @param inventory the inventory to display
 * @param active_section the section of the user experience the user is in
 * @param active_category the current category the user has chosen or has their cursor on
 * @param active_action the current action the user has chosen or has their cursor on
 * @param active_item the current action the user has chosen or has their cursor on
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_inventory(game_window_t * game_window, inventory_t * inventory, section_options_t active_section, category_options_t active_category, action_options_t active_action, unsigned short active_item);

/**
 * @brief displays a go back button and a button for each of the inventory items categories :
 * weapons, armors, health potions and mana potions
 *
 * @param renderer the renderer to use
 * @param icon_size the size of the categories' icons
 * @param padding the padding on the left of the icons
 * @param active_section the section of the user experience the user is in
 * @param active_category the current category the user has chosen or has their cursor on
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_categories(SDL_Renderer * renderer, int icon_size, int padding, section_options_t active_section, category_options_t active_category);

/**
 * @brief displays a grid of 9 items in the current category, and details on the item the cursor is currently on
 *
 * @param renderer the renderer to use
 * @param items_container the rectangle in which to display the grid
 * @param details_container the rectangle in which to display the item details
 * @param type the type of item the cursor is on (armor or weapon)
 * @param inventory the inventory the items are in
 * @param details_font_size the font size for the item details
 * @param active_item the index of the item the cursor is currently on
 * @param active_section the section of the user experience the user is in
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_items(SDL_Renderer * renderer, rect_t * items_container, SDL_Rect details_container, item_types_t type, inventory_t * inventory, int details_font_size, unsigned short active_item, section_options_t active_section);

/**
 * @brief displays details on the selected heal_potion
 *
 * @param renderer the renderer to use
 * @param type the type of heal_potion selected (health or mana)
 * @param details_container the rectangle in which to display the item details
 * @param quantity the quantity of selected potions in the inventory
 * @param details_font_size the font size for the heal_potion details
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_potions(SDL_Renderer * renderer, potion_types_t type, SDL_Rect details_container, unsigned int quantity);

/**
 * @brief displays the possible actions from which to choose: use or throw away
 *
 * @param renderer the renderer to use
 * @param actions_container the rectangle in which to display the actions
 * @param font_size the font size for the actions
 * @param active_action the action the cursor is currently on
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_actions(SDL_Renderer * renderer, SDL_Rect actions_container, int font_size, action_options_t active_action);

/**
 * @brief displays "Nothing to see" in a given container
 *
 * @param renderer the renderer to use
 * @param container the rectangle in which to display the message
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_nothing_to_see(SDL_Renderer * renderer, SDL_Rect container);

/**
 * @brief displays indicators at the right side of a container if the user can scroll up or down
 *
 * @param renderer the renderer to use
 * @param container the rectangle in which the indicator will be
 * @param font_size the font size of the indicator
 * @param items_count the number of items displayed on one page
 * @param quantity the total number of items
 * @param first_item_to_print the index of the first item to be printed on the page
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_scroll_indicator(SDL_Renderer * renderer, SDL_Rect container, int font_size, int items_count, int quantity, int first_item_to_print);

#endif