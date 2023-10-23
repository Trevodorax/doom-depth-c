#ifndef DOOM_DEPTH_C_SHOP_UTILS_H
#define DOOM_DEPTH_C_SHOP_UTILS_H

#include "../shop_screen.h"
#include "stdbool.h"

/**
 * @brief Returns the number of items existing in the game in a given category
 *
 * @param category
 * @returns number of items in category or -1
 */
int get_items_count(shop_category_options category);

#endif //DOOM_DEPTH_C_SHOP_UTILS_H