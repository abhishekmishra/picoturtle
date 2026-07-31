cmake_minimum_required(VERSION 3.20)
project(picoturtle VERSION 0.3.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(PICOTURTLE_CORE_SRC
    c-color-names/color_names.h
    c-color-names/color_names.c
    src/font.h
    src/font.c
    src/image.h
    src/image.c
    src/picoturtle_lua.c
    src/runtime.h
    src/runtime.c
    src/turtle.h
    src/turtle.c
)

add_library(picoturtle_core STATIC ${PICOTURTLE_CORE_SRC})
target_include_directories(picoturtle_core PUBLIC src)
target_include_directories(picoturtle_core PUBLIC c-color-names)

# raylib dependency
find_package(raylib CONFIG REQUIRED)
target_link_libraries(picoturtle_core PUBLIC raylib)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(picoturtle_core PUBLIC glfw)

find_package(Lua REQUIRED)
target_include_directories(picoturtle_core PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(picoturtle_core PUBLIC ${LUA_LIBRARIES})

add_executable(picoturtle src/picoturtle.c)
target_link_libraries(picoturtle PRIVATE picoturtle_core)
target_compile_definitions(
    picoturtle
    PRIVATE
        PICOTURTLE_VERSION="${PROJECT_VERSION}"
        PICOTURTLE_CODENAME="matrix-goose"
)

include(CTest)
if(BUILD_TESTING)
    add_executable(picoturtle_turtle_test test/test_turtle.c)
    target_link_libraries(picoturtle_turtle_test PRIVATE picoturtle_core)
    add_test(NAME picoturtle_turtle_core COMMAND picoturtle_turtle_test)

    add_executable(picoturtle_api_test test/test_lua_api.c)
    target_link_libraries(picoturtle_api_test PRIVATE picoturtle_core)
    add_test(NAME picoturtle_lua_api COMMAND picoturtle_api_test)

    add_executable(picoturtle_samples_test test/test_samples.c)
    target_link_libraries(picoturtle_samples_test PRIVATE picoturtle_core)
    add_test(NAME picoturtle_core_samples COMMAND picoturtle_samples_test)
    set_tests_properties(
        picoturtle_core_samples
        PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    add_test(NAME picoturtle_cli_help COMMAND picoturtle --help)
    add_test(NAME picoturtle_cli_version COMMAND picoturtle --version)
    add_test(NAME picoturtle_cli_missing_output COMMAND picoturtle -o)
    set_tests_properties(picoturtle_cli_missing_output PROPERTIES WILL_FAIL TRUE)
endif()
