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
        src/entities/stage/stage.h
        src/entities/stats/stats.h
        src/entities/weapon/weapon.h
        src/storage/json/json.c
        src/storage/json/json.h
        src/utils/array.c
        src/utils/array.h)

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
        src/storage/json/json_test.c
        src/storage/json/json.h)
