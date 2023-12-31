cmake_minimum_required(VERSION 3.26)
project(doom_depth_c C)

set(CMAKE_C_STANDARD 11)

# cross-platform
set(cross_platform_targets
        src/doom_depth.c
        src/doom_depth.h
        src/entities/armor/armor.h
        src/entities/inventory/inventory.h
        src/entities/monster/monster.h
        src/entities/monster/monster.c
        src/entities/player/player.h
        src/entities/spell/spell.h
        src/entities/fight/fight.h
        src/entities/fight/fight.c
        src/entities/stats/stats.h
        src/entities/weapon/weapon.h
        src/entities/entities.h
        src/entities/entities.c
        src/storage/json/json.c
        src/storage/json/json.h
        src/utils/array.c
        src/utils/array.h
        src/map_screen/map_screen.c
        src/map_screen/map_screen.h
        src/ui_utils/sdl_utils/sdl_utils.c
        src/ui_utils/sdl_utils/sdl_utils.h
        src/storage/database/database.h
        src/storage/database/database.c
        src/storage/database/database_queries.h
        src/storage/database/database_queries.c
        src/entities/player/player.c
        src/entities/spell/spell.c
        src/entities/stats/stats.c
        src/entities/inventory/inventory.c
        src/entities/weapon/weapon.c
        src/entities/armor/armor.c
        src/map_screen/stage/stage.c
        src/map_screen/stage/stage.h
        src/start_menu/start_menu.c
        src/start_menu/start_menu.h
        src/fight_screen/fight_screen.c
        src/fight_screen/fight_screen.h
        src/fight_screen/fight_menu/fight_menu.c
        src/fight_screen/fight_menu/fight_menu.h
        src/fight_screen/fight_actions/fight_actions.c
        src/fight_screen/fight_actions/fight_actions.h
        src/game_over/game_over.h
        src/game_over/game_over.c
        src/utils/router.h
        src/fight_screen/fight_dimensions/fight_dimensions.c
        src/fight_screen/fight_dimensions/fight_dimensions.h
        src/ui_utils/cli_utils/cli_utils.h
        src/game_window/game_window.c
        src/game_window/game_window.h
        src/map_screen/map/map.c
        src/map_screen/map/map.h
        src/map_screen/stage/display/display.c
        src/map_screen/stage/display/display.h
        src/map_screen/map/display/display.c
        src/map_screen/map/display/display.h
        src/event/event.c
        src/event/event.h
        src/utils/utils.c
        src/utils/utils.h
        src/inventory_screen/inventory_screen.c
        src/inventory_screen/inventory_screen.h
        src/inventory_screen/display/display.c
        src/inventory_screen/display/display.h
        src/inventory_screen/utils/inventory_utils.c
        src/inventory_screen/utils/inventory_utils.h
        src/ui_utils/cli_utils/ascii_art/ascii_art.c
        src/ui_utils/cli_utils/ascii_art/ascii_art.h
        src/start_menu/display/display.c
        src/start_menu/display/display.h
        src/game_over/display/display.c
        src/game_over/display/display.h
        src/new_game_screen/new_game_screen.h
        src/new_game_screen/new_game_screen.c
        src/new_game_screen/display/display.h
        src/new_game_screen/display/display.c
        src/select_game_screen/load_game_screen.h
        src/select_game_screen/load_game_screen.c
        src/select_game_screen/delete_game_screen.c
        src/select_game_screen/delete_game_screen.h
        src/select_game_screen/display/display.h
        src/select_game_screen/display/display.c
        src/ui_utils/cli_utils/cli_text/cli_text.h
        src/ui_utils/cli_utils/types.h
        src/fight_screen/fight_utils/fight_utils.c
        src/fight_screen/fight_utils/fight_utils.h
        src/fight_screen/types.h
        src/fight_screen/display/display.c
        src/fight_screen/display/display.h
        src/fight_screen/fight_menu/display/display.c
        src/fight_screen/fight_menu/display/display.h
        src/logs/log.c
        src/logs/log.h
        src/shop_screen/shop_screen.h
        src/shop_screen/shop_screen.c
        src/shop_screen/display/display.c
        src/shop_screen/display/display.h
        src/utils/items_management/types.h
        src/shop_screen/utils/shop_utils.h
        src/shop_screen/utils/shop_utils.c
        src/ui_utils/ui_utils.h
        src/ui_utils/ui_utils.c
        src/fight_screen/display/gui/gui.c
        src/fight_screen/display/gui/gui.h
        src/fight_screen/display/cli/cli.c
        src/fight_screen/display/cli/cli.h
        src/ui_utils/cli_utils/cli_utils.c
        src/fight_screen/fight_menu/display/cli/cli.c
        src/fight_screen/fight_menu/display/cli/cli.h
        src/fight_screen/fight_menu/display/gui/gui.c
        src/fight_screen/fight_menu/display/gui/gui.h
        src/inventory_screen/display/gui/gui.c
        src/inventory_screen/display/gui/gui.h
        src/inventory_screen/display/cli/cli.c
        src/inventory_screen/display/cli/cli.h
        src/shop_screen/display/gui/gui.c
        src/shop_screen/display/gui/gui.h
        src/shop_screen/display/cli/cli.c
        src/shop_screen/display/cli/cli.h
        src/entities/treasure/treasure.c
        src/entities/treasure/treasure.h
        src/level_up_screen/level_up_screen.c
        src/level_up_screen/level_up_screen.h
        src/level_up_screen/types.h
        src/level_up_screen/display/display.c
        src/level_up_screen/display/display.h
        src/level_up_screen/display/gui/gui.c
        src/level_up_screen/display/gui/gui.h
        src/level_up_screen/display/cli/cli.c
        src/level_up_screen/display/cli/cli.h
        src/ui_utils/themes/themes.h
        src/ui_utils/themes/themes.c
        src/ui_utils/themes/types.h
        src/help_screen/help_screen.c
        src/help_screen/help_screen.h
        src/help_screen/display/gui/gui.c
        src/help_screen/display/gui/gui.h
        src/help_screen/display/cli/cli.c
        src/help_screen/display/cli/cli.h
        src/help_screen/display/display.c
        src/help_screen/display/display.h
        src/confirm_quit_screen/confirm_quit_screen.h
        src/confirm_quit_screen/confirm_quit_screen.c
        src/confirm_quit_screen/display/display.h
        src/confirm_quit_screen/display/display.c
        src/stats_screen/display/display.c
        src/stats_screen/display/display.h
        src/stats_screen/display/cli/cli.c
        src/stats_screen/display/cli/cli.h
        src/stats_screen/display/gui/gui.c
        src/stats_screen/display/gui/gui.h
        src/stats_screen/stats_screen.c
        src/stats_screen/stats_screen.h
        src/map_generation/map_generation.c
        src/map_generation/map_generation.h
        src/game_stats_screen/game_stats_screen.h
        src/game_stats_screen/game_stats_screen.c
        src/game_stats_screen/display/gui/gui.h
        src/game_stats_screen/display/gui/gui.c
        src/game_stats_screen/types.h
        src/game_stats_screen/display/cli/cli.h
        src/game_stats_screen/display/cli/cli.c
        src/game_stats_screen/display/display.h
        src/game_stats_screen/display/display.c
)

# for unix
set(unix_targets
        src/ui_utils/cli_utils/cli_utils_unix.c
        src/ui_utils/cli_utils/cli_text/cli_text_unix.c
)

# for windows
set(windows_targets
        src/ui_utils/cli_utils/cli_utils_windows.c
        src/ui_utils/cli_utils/cli_text/cli_text_windows.c
)

# cross-platform tests
set(test_targets
        test.c
        src/minunit/minunit.h
        src/storage/json/json_test.c
        src/storage/json/json_test.h
        src/utils/array_test.c
        src/utils/array_test.h
        src/storage/database/database_test.c
        src/storage/database/database_test.h
        src/map_screen/map/map_test.c
        src/map_screen/map/map_test.h
        src/ui_utils/cli_utils/ascii_art/ascii_art_test.c
        src/ui_utils/cli_utils/ascii_art/ascii_art_test.h
        src/logs/log.c
        src/logs/log.h
)

# unix tests
set(test_unix_targets
)

# windows tests
set(test_windows_targets
)

add_executable(doom_depth_c_unix
        main.c
        ${cross_platform_targets}
        ${unix_targets})
add_executable(doom_depth_c_windows
        main.c
        ${cross_platform_targets}
        ${windows_targets})
add_executable(doom_depth_tests_unix
        ${cross_platform_targets}
        ${test_targets}
        ${test_unix_targets}
        ${unix_targets})
add_executable(doom_depth_tests_windows
        ${cross_platform_targets}
        ${test_targets}
        ${test_windows_targets}
        ${windows_targets})

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

# List of all targets
set(TARGETS doom_depth_c_unix doom_depth_c_windows doom_depth_tests_unix doom_depth_tests_windows)

# Link sqlite3 to all targets
foreach(TARGET ${TARGETS})
    target_link_libraries(${TARGET} PRIVATE unofficial::sqlite3::sqlite3)
endforeach()

# List of all SDL libraries
set(SDL_LIBS
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
)

# Link SDL libraries to all targets
foreach(TARGET ${TARGETS})
    target_link_libraries(${TARGET} PRIVATE ${SDL_LIBS})
endforeach()