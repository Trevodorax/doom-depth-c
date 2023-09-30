cmake_minimum_required(VERSION 3.26)
project(doom_depth_c C)

set(CMAKE_C_STANDARD 11)

add_executable(doom_depth_c
        main.c
        src/doom_depth.c
        src/doom_depth.h
        src/entities/armor/armor.h
        src/entities/inventory/inventory.h
        src/entities/monster/monster.h
        src/entities/player/player.h
        src/entities/spell/spell.h
        src/entities/fight/fight.h
        src/entities/fight/fight.c
        src/entities/stats/stats.h
        src/entities/weapon/weapon.h
        src/storage/json/json.c
        src/storage/json/json.h
        src/utils/array.c
        src/utils/array.h
        src/map/map.c
        src/map/map.h
        src/sdl_utils/sdl_utils.c
        src/sdl_utils/sdl_utils.h
        src/storage/database/database.h
        src/storage/database/database.c
        src/storage/database/database_queries.h
        src/storage/database/database_queries.c
        src/map/stage/stage.c
        src/map/stage/stage.h
        src/start_menu/start_menu.c
        src/start_menu/start_menu.h)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(doom_depth_c PRIVATE unofficial::sqlite3::sqlite3)
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(doom_depth_c
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

add_executable(doom_depth_tests
        test.c
        src/minunit/minunit.h
        src/storage/json/json.c
        src/storage/json/json.h
        src/storage/json/json_test.c
        src/storage/json/json_test.h
        src/utils/array.c
        src/utils/array.h
        src/utils/array_test.c
        src/utils/array_test.h
        src/storage/database/database.h
        src/storage/database/database.c
        src/storage/database/database_queries.h
        src/storage/database/database_queries.c
        src/storage/database/database_test.c
        src/storage/database/database_test.h
        src/sdl_utils/sdl_utils.c
        src/sdl_utils/sdl_utils.h
        src/map/stage/stage.c
        src/map/stage/stage.h
        src/entities/fight/fight.c
        src/entities/fight/fight.h
        src/map/map.c
        src/map/map.h
        src/map/map_test.c
        src/map/map_test.h

)

target_link_libraries(doom_depth_tests PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(doom_depth_tests
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

find_package(SDL2_ttf CONFIG REQUIRED)
target_link_libraries(doom_depth_tests
        PRIVATE
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(doom_depth_tests
        PRIVATE
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
)

find_package(SDL2_ttf CONFIG REQUIRED)
target_link_libraries(doom_depth_c
        PRIVATE
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(doom_depth_c
        PRIVATE
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
)