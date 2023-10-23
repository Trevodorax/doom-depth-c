#include "display.h"
#include "../../sdl_utils/sdl_utils.h"

int display_shop_gui(game_window_t *game_window, player_t *player);
int display_shop_cli(game_window_t *game_window, player_t *player);

int display_shop(game_window_t *game_window, player_t *player) {
    switch (game_window->ui_type) {
        case GUI:
            return display_shop_gui(game_window, player);
        case CLI:
            return display_shop_cli(game_window, player);
    }
}

int display_shop_gui(game_window_t *game_window, player_t *player) {

}

int display_shop_cli(game_window_t *game_window, player_t *player){

}