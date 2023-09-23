cmake_minimum_required(VERSION 3.26)
project(doom_depth_c C)

set(CMAKE_C_STANDARD 11)

add_executable(doom_depth_c main.c)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(doom_depth_c PRIVATE unofficial::sqlite3::sqlite3)
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(doom_depth_c
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)