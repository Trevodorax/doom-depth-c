#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "sdl_utils.h"

int draw_image_in_rectangle_multiple_sprites(SDL_Renderer *renderer, SDL_Rect container, const char *file_path);
int draw_image_in_rectangle_single_sprite(SDL_Renderer *renderer, SDL_Rect container, const char *file_path, orientation_t orientation);

int set_draw_color(SDL_Renderer * renderer, SDL_Color color) {
    if (SDL_SetRenderDrawColor(
            renderer,
            color.r,
            color.g,
            color.b,
            color.a
    ) != 0)
    {
        fprintf(stderr, "\nSDL_SetRenderDrawColor error : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
};

int set_background_color(SDL_Renderer * renderer, SDL_Color color)
{
    if(set_draw_color(renderer, color) != 0)
    {
        return EXIT_FAILURE;
    }

    if(SDL_RenderClear(renderer) != 0)
    {
        fprintf(stderr, "\nSDL_RenderClear error : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

SDL_Texture * get_bmp_texture(SDL_Renderer * renderer, const char * image_file_name) {
    SDL_Surface * img_surface = NULL;
    SDL_Texture * img_texture = NULL;

    if(!image_file_name)
    {
        return NULL;
    }

    // get the image surface
    img_surface = SDL_LoadBMP(image_file_name);
    if(!img_surface)
    {
        return NULL;
    }

    // transform the surface into a texture
    img_texture = SDL_CreateTextureFromSurface(renderer, img_surface);
    if(!img_texture)
    {
        fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }

    // free the surface (it was just to create the texture)
    SDL_FreeSurface(img_surface);

    return img_texture;
}

int draw_fill_rect(
        SDL_Rect rect,
        SDL_Color color,
        SDL_Renderer * renderer
)
{
    if(set_draw_color(renderer, color) != 0)
    {
        return EXIT_FAILURE;
    }

    if(SDL_RenderFillRect(renderer, &rect) != 0)
    {
        fprintf(stderr, "\nSDL_RenderFillRect error : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int draw_thick_rect(
        SDL_Rect rect,
        unsigned int thickness,
        SDL_Color color,
        SDL_Renderer * renderer
)
{
    if(SDL_SetRenderDrawColor(
            renderer,
            color.r,
            color.g,
            color.b,
            color.a
    ) != 0)
    {
        fprintf(stderr, "\nSDL_SetRenderDrawColor error : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Rect current_container_rect;

    for(int i = 0; i < thickness; i++)
    {
        current_container_rect.x = rect.x - i;
        current_container_rect.y = rect.y - i;
        current_container_rect.w = rect.w + 2 * i;
        current_container_rect.h = rect.h + 2 * i;

        if(SDL_RenderDrawRect(
                renderer,
                &current_container_rect
        ) != 0)
        {
            fprintf(stderr, "\nSDL_RenderDrawRect error : %s", SDL_GetError());
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int draw_image_in_rectangle(SDL_Renderer *renderer, SDL_Rect container, const char *file_path, orientation_t orientation) {
    char * orientated_file_path = malloc(strlen(file_path) + 10);
    if (!orientated_file_path) {
        return EXIT_FAILURE;
    }

    bool has_directions = false;

    // build the orientated_file_path based on orientation
    const char *direction = NULL;
    switch (orientation) {
        case NORTH:
            direction = "_north.png";
            break;
        case EAST:
            direction = "_east.png";
            break;
        case SOUTH:
            direction = "_south.png";
            break;
        case WEST:
            direction = "_west.png";
            break;
        default:
            // Handle invalid orientation
            free(orientated_file_path);
            return EXIT_FAILURE;
    }

    strcpy(orientated_file_path, file_path);
    char * dot = strrchr(orientated_file_path, '.');
    if (dot) {
        *dot = '\0';
        strcat(orientated_file_path, direction);
    }

    FILE *file = fopen(orientated_file_path, "r");
    if (file) {
        // File exists
        has_directions = true;
        fclose(file);
    } else {
        // File doesn't exist
        has_directions = false;
    }

    if (has_directions) {
        draw_image_in_rectangle_multiple_sprites(renderer, container, orientated_file_path);
    } else {
        draw_image_in_rectangle_single_sprite(renderer, container, file_path, orientation);
    }

    free(orientated_file_path);

    return EXIT_SUCCESS;
}

int draw_image_in_rectangle_multiple_sprites(SDL_Renderer *renderer, SDL_Rect container, const char *file_path) {
    SDL_Texture *image_texture = get_image_texture(renderer, file_path);
    if (!image_texture) {
        return EXIT_FAILURE;
    }

    SDL_RenderCopy(renderer, image_texture, NULL, &container);

    return EXIT_SUCCESS;
}

int draw_image_in_rectangle_single_sprite(SDL_Renderer *renderer, SDL_Rect container, const char *file_path, orientation_t orientation) {
    SDL_Texture *image_texture = get_image_texture(renderer, file_path);
    if (!image_texture) {
        return EXIT_FAILURE;
    }

    double angle = 0.0;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    switch (orientation) {
        case NORTH:
            angle = 0.0;
            flip = SDL_FLIP_NONE;
            break;
        case EAST:
            angle = 270.0;
            flip = SDL_FLIP_NONE;
            break;
        case SOUTH:
            angle = 180.0;
            flip = SDL_FLIP_NONE;
            break;
        case WEST:
            angle = 90.0;
            flip = SDL_FLIP_NONE;
            break;
        default:
            SDL_DestroyTexture(image_texture);
            return EXIT_FAILURE;
    }

    SDL_Point center = {container.w / 2, container.h / 2};

    SDL_RenderCopyEx(renderer, image_texture, NULL, &container, angle, &center, flip);
    SDL_DestroyTexture(image_texture);

    return EXIT_SUCCESS;
}

SDL_Texture * get_string_texture(SDL_Renderer * renderer, const char * string, const char * font_path, int font_size, SDL_Color color) {
    SDL_Surface * string_surface = NULL;
    SDL_Texture * string_texture = NULL;

    TTF_Font *font = TTF_OpenFont(font_path, font_size);
    if (!font) {
        fprintf(stderr, "\nError TTF_OpenFont : %s", TTF_GetError());
        return NULL;
    }

    if(!string)
    {
        return NULL;
    }

    // get the string surface
    string_surface = TTF_RenderText_Solid(font, string, color);
    if(!string_surface)
    {
        return NULL;
    }

    // transform the surface into a texture
    string_texture = SDL_CreateTextureFromSurface(renderer, string_surface);
    if(!string_texture)
    {
        fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }

    // free the surface (it was just to create the texture)
    SDL_FreeSurface(string_surface);

    TTF_CloseFont(font);
    return string_texture;
}

SDL_Texture * get_image_texture(SDL_Renderer * renderer, const char * image_path) {
    SDL_Surface * image_surface = NULL;
    SDL_Texture * image_texture = NULL;
    if (!image_path) {
        return NULL;
    }

    // get the image surface
    image_surface = IMG_Load(image_path);
    if(!image_surface)
    {
        return NULL;
    }

    // transform the surface into a texture
    image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    if(!image_texture)
    {
        fprintf(stderr, "\nError SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }

    // free the surface (it was just to create the texture)
    SDL_FreeSurface(image_surface);

    return image_texture;
}

SDL_Rect *get_rectangle_grid(int nb_rectangles, SDL_Rect *container) {
    if (nb_rectangles <= 0 || container == NULL) {
        return NULL;
    }

    int rows = (int)sqrt(nb_rectangles);
    int cols = (nb_rectangles + rows - 1) / rows; // calculate columns based on rows

    // Calculate width and height for each cell with a small margin
    int cell_width = (container->w / cols) - 2;
    int cell_height = (container->h / rows) - 2;

    SDL_Rect *rect_grid = malloc(rows * cols * sizeof(SDL_Rect));
    if (!rect_grid) {
        fprintf(stderr, "\nget_rectangle_grid error: couldn't allocate memory");
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int index = i * cols + j; // calculate the index in the 1D array
            rect_grid[index].x = container->x + j * (cell_width + 2);
            rect_grid[index].y = container->y + i * (cell_height + 2);
            rect_grid[index].w = cell_width;
            rect_grid[index].h = cell_height;
        }
    }

    return rect_grid;
}

