cmake_minimum_required(VERSION 3.21)
project(picoturtle VERSION 0.3.0 LANGUAGES C)

include(GNUInstallDirs)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(PICOTURTLE_CORE_SRC
    third_party/c-color-names/color_names.h
    third_party/c-color-names/color_names.c
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
target_include_directories(
    picoturtle_core
    PUBLIC third_party/c-color-names
)

# raylib dependency
find_package(raylib CONFIG REQUIRED)
target_link_libraries(picoturtle_core PUBLIC raylib)

find_package(Lua REQUIRED)
target_include_directories(picoturtle_core PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(picoturtle_core PUBLIC ${LUA_LIBRARIES})

add_executable(picoturtle src/picoturtle.c)
target_link_libraries(picoturtle PRIVATE picoturtle_core)
if(APPLE)
    set_target_properties(
        picoturtle
        PROPERTIES INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}"
    )
elseif(UNIX)
    set_target_properties(
        picoturtle
        PROPERTIES INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
    )
endif()
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
    add_test(
        NAME picoturtle_cli_integration
        COMMAND
            ${CMAKE_COMMAND}
            -DPICOTURTLE=$<TARGET_FILE:picoturtle>
            -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
            -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/test/test_cli.cmake
    )
    add_test(
        NAME picoturtle_resource_lifetime
        COMMAND
            picoturtle
            --no-wait
            ${CMAKE_CURRENT_SOURCE_DIR}/test/resource_lifetime.lua
    )
    set_tests_properties(
        picoturtle_resource_lifetime
        PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

install(
    TARGETS picoturtle
    RUNTIME_DEPENDENCY_SET picoturtle_runtime_dependencies
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
    RUNTIME_DEPENDENCY_SET picoturtle_runtime_dependencies
    PRE_EXCLUDE_REGEXES
        "^/System/Library/"
        "^/usr/lib/"
    POST_EXCLUDE_REGEXES
        "^/System/Library/"
        "^/usr/lib/"
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
    DIRECTORY lua/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/picoturtle/lua
)
install(
    DIRECTORY res/fonts res/images
    DESTINATION ${CMAKE_INSTALL_DATADIR}/picoturtle/res
)
install(
    DIRECTORY docs/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/picoturtle/docs
)
install(
    FILES
        README.md
        API_COMPATIBILITY.md
        CHANGELOG.md
        LICENSE
        LICENSE-Lua
        THIRD_PARTY_NOTICES.md
    DESTINATION ${CMAKE_INSTALL_DATADIR}/picoturtle
)
install(
    DIRECTORY third_party/c-color-names/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/picoturtle/licenses/c-color-names
    FILES_MATCHING
        PATTERN "LICENSE"
        PATTERN "UPSTREAM.md"
)
