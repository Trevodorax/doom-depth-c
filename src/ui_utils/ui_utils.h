#ifndef DOOM_DEPTH_C_UI_UTILS_H
#define DOOM_DEPTH_C_UI_UTILS_H

#include "../ui_utils/cli_utils/types.h"
#include "../ui_utils/sdl_utils/sdl_utils.h"

/**
 * @brief Get a list of rectangles that form a grid
 *
 * The grid is made of an equal number of rows and columns (or almost equal) with a bit of margin between each
 * All these rectangles are contained within the container rectangle.
 *
 * @param nb_rectangles The number of rectangles in the grid
 * @param container Rectangle in which all the rectangles of the grid should be
 * @return The array of rectangles that form the grid
 */
rect_t *get_rectangle_grid(size_t nb_rectangles, rect_t *container);

/**
 * @brief Get a list of rectangles that form a specified layout
 *
 * @param nb_rectangles The number of rectangles in the grid
 * @param container Rectangle in which all the rectangles of the grid should be
 * @param layout The specified layout
 * @return The array of rectangles forming the layout
 */
rect_t *get_rectangle_layout(size_t nb_rectangles, rect_t *container, layout_t layout, int margin);

#endif //DOOM_DEPTH_C_UI_UTILS_H