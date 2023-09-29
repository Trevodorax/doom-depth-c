#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "sdl_utils.h"

void free_game_window(game_window_t * game_window) {
    if(game_window->window)
    {
        SDL_DestroyWindow(game_window->window);
    }
    if(game_window->renderer)
    {
        SDL_DestroyRenderer(game_window->renderer);
    }
    if(game_window->context) {
        free(game_window->context);
    }

    TTF_Quit();
}

game_window_t * init_game_window() {
    game_window_t *game_window = malloc(sizeof(game_window_t));
    if(!game_window) {
        return NULL;
    }

    game_window->window = SDL_CreateWindow(
            "Doom depth c",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            700,
            500,
            SDL_WINDOW_RESIZABLE
    );
    if(!game_window->window) {
        return NULL;
    }

    game_window->renderer = SDL_CreateRenderer(
            game_window->window,
            1,
            SDL_RENDERER_ACCELERATED
    );
    if(!game_window->renderer) {
        return NULL;
    }

    game_window->context = malloc(sizeof(window_context_t));
    if(!game_window->context) {
        return NULL;
    }

    if (TTF_Init()) {
        return NULL;
    }

    return game_window;
}

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

int drawThickRect(
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


SDL_Texture * get_string_texture(SDL_Renderer * renderer, const char * string, const char * font_path, int font_size, SDL_Color color) {
    SDL_Surface * string_surface = NULL;
    SDL_Texture * string_texture = NULL;
    TTF_Font *font = TTF_OpenFont(font_path, font_size);

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