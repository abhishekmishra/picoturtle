cmake_minimum_required(VERSION 3.10)
project(ptrl C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(PTRL_CORE_SRC
    ../c-color-names/color_names.h
    ../c-color-names/color_names.c
    src/ptrtllua.c
    src/runtime.h
    src/runtime.c
    src/turtle.h
    src/turtle.c
)

add_library(ptrl_core STATIC ${PTRL_CORE_SRC})
target_include_directories(ptrl_core PUBLIC src)
target_include_directories(ptrl_core PUBLIC ../c-color-names)

# raylib dependency
find_package(raylib CONFIG REQUIRED)
target_link_libraries(ptrl_core PUBLIC raylib)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(ptrl_core PUBLIC glfw)

find_package(Lua REQUIRED)
target_include_directories(ptrl_core PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(ptrl_core PUBLIC ${LUA_LIBRARIES})

add_executable(ptrl src/ptrtl.c)
target_link_libraries(ptrl PRIVATE ptrl_core)

include(CTest)
if(BUILD_TESTING)
    add_executable(ptrl_turtle_test test/test_turtle.c)
    target_link_libraries(ptrl_turtle_test PRIVATE ptrl_core)
    add_test(NAME ptrl_turtle_core COMMAND ptrl_turtle_test)

    add_executable(ptrl_api_test test/test_lua_api.c)
    target_link_libraries(ptrl_api_test PRIVATE ptrl_core)
    add_test(NAME ptrl_lua_api COMMAND ptrl_api_test)

    add_executable(ptrl_samples_test test/test_samples.c)
    target_link_libraries(ptrl_samples_test PRIVATE ptrl_core)
    add_test(NAME ptrl_core_samples COMMAND ptrl_samples_test)
    set_tests_properties(
        ptrl_core_samples
        PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
    )

    add_test(NAME ptrl_cli_help COMMAND ptrl --help)
    add_test(NAME ptrl_cli_missing_output COMMAND ptrl -o)
    set_tests_properties(ptrl_cli_missing_output PROPERTIES WILL_FAIL TRUE)
endif()
