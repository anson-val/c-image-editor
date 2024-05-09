cmake_minimum_required(VERSION 3.28)
project(c_image_editor C)

set(CMAKE_C_STANDARD 23)

add_executable(c-image-editor main.c
        operations.c
        operations.h
        bmp.c
        bmp.h
        utils.c
        utils.h
)

target_link_libraries(c-image-editor m)