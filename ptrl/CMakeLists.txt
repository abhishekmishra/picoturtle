cmake_minimum_required(VERSION 3.10)
project(ptrl C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(PTRL_SRC
    ../c-color-names/color_names.h
    ../c-color-names/color_names.c
    src/turtle.h
    src/turtle.c
    src/ptrtl.c
)

add_executable(ptrl ${PTRL_SRC})
target_include_directories(ptrl PRIVATE src)
target_include_directories(ptrl PRIVATE ../c-color-names)

# raylib dependency
find_package(raylib CONFIG REQUIRED)
target_link_libraries(ptrl PRIVATE raylib)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(ptrl PRIVATE glfw)

find_package(Lua REQUIRED)
target_include_directories(ptrl PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(ptrl PUBLIC ${LUA_LIBRARIES})

find_package(coll CONFIG REQUIRED)
target_link_libraries(ptrl PUBLIC coll::coll)

find_package(zclk CONFIG REQUIRED)
target_link_libraries(ptrl PUBLIC zclk::zclk)