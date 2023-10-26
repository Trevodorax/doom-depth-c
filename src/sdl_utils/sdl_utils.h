#ifndef DOOM_DEPTH_C_SDL_UTILS_H
#define DOOM_DEPTH_C_SDL_UTILS_H

#include <SDL2/SDL.h>
#include "../entities/player/player.h"
#include "../map_screen//stage/stage.h"
#include "../utils/router.h"
#include "../cli_utils/cli_utils.h"

typedef enum {
    GRID,
    HORIZONTAL,
    VERTICAL
} layout_t;

/**
 * @brief Sets the drawing color for a renderer
 *
 * This function sets the drawing color for the specified renderer
 *
 * @param renderer Pointer to the renderer for which the draw color is set
 * @param color The color to set
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int set_draw_color(SDL_Renderer * renderer, SDL_Color color);

/**
 * @brief Sets the background color for a renderer
 *
 * Sets the background color for the specified renderer and clears it
 *
 * @param renderer Pointer to the renderer for which the background color is set
 * @param color The color to set as the background
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int set_background_color(SDL_Renderer * renderer, SDL_Color color);

/**
 * @brief Loads an image and returns it as a texture
 *
 * Loads an image from a file and converts it into a texture
 * that can be used with the specified renderer (the file has to be in BMP format)
 *
 * @param renderer Pointer to the renderer
 * @param image_file_name Name of the image file to load
 * @return Pointer to the created texture, or NULL on failure
 */
SDL_Texture * get_bmp_texture(SDL_Renderer * renderer, const char * image_file_name);

/**
 * @brief Draws a filled rectangle with the specified color
 *
 * @param rect The rectangle to be filled and drawn
 * @param color The color of the filled rectangle
 * @param renderer Pointer to the renderer
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int draw_fill_rect(SDL_Rect rect, SDL_Color color, SDL_Renderer * renderer);

/**
 * @brief Draws a thick-bordered rectangle with the specified color and thickness.
 *
 * Draws a rectangle with a thick border on the renderer with
 * the specified color, rectangle parameters, and border thickness.
 *
 * @param rect The rectangle to be drawn.
 * @param thickness The thickness of the rectangle border.
 * @param color The color of the rectangle border.
 * @param renderer Pointer to the renderer.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
int draw_thick_rect(SDL_Rect rect, unsigned int thickness, SDL_Color color, SDL_Renderer * renderer);

/**
 * @brief Draws an image in a rectangle with specified orientation
 *
 * @param renderer   Pointer to the SDL renderer
 * @param container  Rectangle in which to draw the image
 * @param file_path  Path to the image file
 * @param orientation Desired orientation of the image
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int
draw_image_in_rectangle(SDL_Renderer *renderer, SDL_Rect container, const char *file_path, orientation_t orientation,
                        bool keep_aspect_ratio, alignment_t x_align, alignment_t y_align);

/**
 * @brief Prints a texture in a rectangle
 * @param renderer
 * @param texture The texture to print
 * @param container The container to print the texture in
 * @param angle The angle of rotation
 * @param center The center of rotation
 * @param keep_aspect_ratio If the aspect ratio of the texture should be preserved
 * @param x_align Horizontal alignment
 * @param y_align Vertical alignment
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int print_texture_in_rectangle(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect container, double angle,
                               SDL_Point center, bool keep_aspect_ratio, alignment_t x_align, alignment_t y_align);

/**
 * @brief Returns string as a texture with the specified font, font size and color.
 *
 * Creates a texture from the given string, accepts new line caracters.
 *
 * @param renderer Pointer to the renderer
 * @param string String to render
 * @param font_path Path of the font for the string
 * @param font_size Size of the font for the string (in pt)
 * @param color Color of the string
 * @return Pointer to the created texture, or NULL on failure
 */
SDL_Texture * get_string_texture(SDL_Renderer * renderer, const char * string, const char * font_path, int font_size, SDL_Color color);

/**
 * @brief Returns image as a texture.
 *
 * Creates a texture from the given image, supported formats are:
 * - JPG (.jpg or .jpeg)
 * - PNG
 * - GIF
 * - BMP
 * - TGA
 * - XPM
 * - PNM
 * - PCX
 * - TIF (.tif ot .tiff)
 * - LBM (.lbm or .iff)
 *
 * @param renderer Pointer to the renderer
 * @param image_path String to render
 * @return Pointer to the created texture, or NULL on failure
 */
SDL_Texture * get_image_texture(SDL_Renderer * renderer, const char * image_path);

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
SDL_Rect *get_rectangle_grid(size_t nb_rectangles, SDL_Rect *container);

/**
 * @brief Get a list of rectangles that form a specified layout
 *
 * @param nb_rectangles The number of rectangles in the grid
 * @param container Rectangle in which all the rectangles of the grid should be
 * @param layout The specified layout
 * @return The array of rectangles forming the layout
 */
SDL_Rect * get_rectangle_layout(size_t nb_rectangles, SDL_Rect * container, layout_t layout);

#endif //DOOM_DEPTH_C_SDL_UTILS_H
