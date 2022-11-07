cmake_minimum_required ( VERSION 3.20 )

project ( picoturtle VERSION 0.1.0 LANGUAGES CXX )

set (VERBOSE)

# Export all symbols for shared linking to work on windows
# see https://stackoverflow.com/questions/37128555/getting-cmake-to-build-shared-library-for-msvc
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

set( CMAKE_CXX_FLAGS " -pthread " )

# Search in the "cmake" directory for the additional CMake modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# It's useful to setup the current version of our code in the build system
# using a `semver` style
set ( PICOTURTLE_VERSION_MAJOR 0 )
set ( PICOTURTLE_VERSION_MINOR 1 )
set ( PICOTURTLE_VERSION_PATCH 0 )

# https://stackoverflow.com/a/63037953
include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(pthread pthread_create "" HAVE_LIB_PTHREAD)

if (HAVE_LIB_PTHREAD)
  set(EXTRA_LIBS ${EXTRA_LIBS} pthread)
endif (HAVE_LIB_PTHREAD)

CHECK_LIBRARY_EXISTS(m sin "" HAVE_LIB_M)

if (HAVE_LIB_M)
  set(EXTRA_LIBS ${EXTRA_LIBS} m)
endif (HAVE_LIB_M)

# https://stackoverflow.com/a/47260387/9483968
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set ( PICOTURTLE_LUA_FILES
  lua/TurtleInit.lua
  lua/SampleTurtle.lua
  lua/runSamples.lua
  lua/samples/empty.lua
  lua/samples/fern.lua
  lua/samples/polygons.lua
  lua/samples/sierpinski.lua
  lua/samples/spiral.lua
  lua/samples/text-demo.lua
  lua/samples/tree.lua
  lua/samples/turtle_basic.lua
  lua/mazes/maze.lua
  lua/mazes/genmazes.lua
)

foreach( LUA_FILE IN LISTS PICOTURTLE_LUA_FILES )
  message("---> copying ${LUA_FILE}")
  # copy from source ${LUA_FILE} to desination ${LUA_FILE}, without any changes
  configure_file( ${LUA_FILE} ${LUA_FILE} COPYONLY )
endforeach()

#--------------------------PicoTurtle Lib---------------------------#

set ( PICOTURTLE_LIB_SOURCES
  src/TurtleColor.hpp
  src/TurtleColor.cpp
  src/TurtleLocation.hpp
  src/TurtleLocation.cpp
  src/TurtleDimension.hpp
  src/TurtleDimension.cpp
  src/TurtleState.hpp
  src/TurtleState.cpp
  src/TurtleOptions.hpp
  src/TurtleOptions.cpp
  src/Turtle.hpp
  src/Turtle.cpp
  src/PicoTurtle.hpp
  src/PicoTurtle.cpp
  src/PicoTurtleLua.hpp
  src/PicoTurtleLua.cpp
)

add_library(picoturtle SHARED ${PICOTURTLE_LIB_SOURCES})
set_property(TARGET picoturtle PROPERTY CXX_STANDARD 17)

if(WIN32)
  if (NOT TARGET skia)
    find_package(skia CONFIG REQUIRED)
  endif()

    # see https://github.com/microsoft/vcpkg/issues/25774#issue-1304459755
    # UNCOMMENT TO FIX LINK ERRORS
    get_target_property(skia_interface_defs skia
    INTERFACE_COMPILE_DEFINITIONS)
    string(REPLACE "SKIA_IMPLEMENTATION=1" "SKIA_IMPLMENTATION=0"
    skia_interface_defs_fixed "${skia_interface_defs}")
    set_target_properties(skia PROPERTIES INTERFACE_COMPILE_DEFINITIONS
    "${skia_interface_defs_fixed}")

    target_include_directories(skia INTERFACE "$ENV{VCPKG_ROOT}/installed/x64-windows/include/skia")
	target_link_libraries(picoturtle PUBLIC skia skia::skia)
endif(WIN32)

if(APPLE)
  find_package(skia CONFIG REQUIRED)

    # see https://github.com/microsoft/vcpkg/issues/25774#issue-1304459755
    # UNCOMMENT TO FIX LINK ERRORS
    get_target_property(skia_interface_defs skia
    INTERFACE_COMPILE_DEFINITIONS)
    string(REPLACE "SKIA_IMPLEMENTATION=1" "SKIA_IMPLMENTATION=0"
    skia_interface_defs_fixed "${skia_interface_defs}")
    set_target_properties(skia PROPERTIES INTERFACE_COMPILE_DEFINITIONS
    "${skia_interface_defs_fixed}")

  target_include_directories(skia INTERFACE "/Users/abhishekmishra/vcpkg/installed/x64-osx/include/skia/")
  target_link_libraries(picoturtle PUBLIC skia skia::skia)
  # skiaConfig.cmake generated in vcpkg only looks for dll file.
  # therefore adding the dynamic library file manually
  target_link_libraries(picoturtle PUBLIC "/Users/abhishekmishra/vcpkg/installed/x64-osx/lib/libskia.dylib")


  message(CHECK_START "Finding skia")
  if (skia_FOUND)
    message(CHECK_PASS "skia found")
  else()
    message(CHECK_FAIL "skia NOT found")
  endif()
endif(APPLE)

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
  set(OpenGL_GL_PREFERENCE GLVND)
  find_package(OpenGL)

  link_directories(/usr/lib/x86_64-linux-gnu/)

  target_link_libraries(picoturtle PUBLIC OpenGL::OpenGL OpenGL::GLX GLX OpenGL)

#  if (NOT TARGET skia)
#  	include(FindSkia)
#    target_include_directories(skia INTERFACE
#      ${SKIA_DIR}/include
#    )
#    target_link_libraries(picoturtle PUBLIC skia)
#  endif()
  find_package(skia CONFIG REQUIRED)

    # see https://github.com/microsoft/vcpkg/issues/25774#issue-1304459755
    # UNCOMMENT TO FIX LINK ERRORS
    get_target_property(skia_interface_defs skia
    INTERFACE_COMPILE_DEFINITIONS)
    string(REPLACE "SKIA_IMPLEMENTATION=1" "SKIA_IMPLMENTATION=0"
    skia_interface_defs_fixed "${skia_interface_defs}")
    set_target_properties(skia PROPERTIES INTERFACE_COMPILE_DEFINITIONS
    "${skia_interface_defs_fixed}")

  target_include_directories(skia INTERFACE "$ENV{VCPKG_ROOT}/installed/x64-linux/include/skia/")
  target_link_libraries(picoturtle PUBLIC skia skia::skia)
  # skiaConfig.cmake generated in vcpkg only looks for dll file.
  # therefore adding the dynamic library file manually
  target_link_libraries(picoturtle PUBLIC "$ENV{VCPKG_ROOT}/installed/x64-linux/lib/libskia.a")
  target_link_libraries(picoturtle PUBLIC "$ENV{VCPKG_ROOT}/installed/x64-linux/lib/libfontconfig.a")

    find_package(PNG REQUIRED)
    target_link_libraries(picoturtle PRIVATE PNG::PNG)

    find_package(WebP CONFIG REQUIRED)
    target_link_libraries(picoturtle PRIVATE WebP::webp WebP::webpdemux WebP::libwebpmux WebP::webpdecoder)

    find_package(LibXml2 REQUIRED)
    target_link_libraries(picoturtle PRIVATE LibXml2::LibXml2)

    find_package(JPEG REQUIRED)
    target_link_libraries(picoturtle PRIVATE ${JPEG_LIBRARIES})
    target_include_directories(picoturtle PRIVATE ${JPEG_INCLUDE_DIR})

    find_package(BZip2 REQUIRED)
    target_link_libraries(picoturtle PRIVATE BZip2::BZip2)

    find_package(expat CONFIG REQUIRED)
    target_link_libraries(picoturtle PRIVATE expat::expat)

    find_package(freetype CONFIG REQUIRED)
    target_link_libraries(picoturtle PRIVATE freetype)

message(CHECK_START "Finding skia")
  if (skia_FOUND)
    message(CHECK_PASS "skia found")
  else()
    message(CHECK_FAIL "skia NOT found")
  endif()
endif()

find_package(Lua REQUIRED)
target_include_directories(picoturtle PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(picoturtle PUBLIC ${LUA_LIBRARIES})

#--------------------------PicoTurtle App---------------------------#
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set ( PICOTURTLE_APP_SOURCES
  src/TurtleCanvasWidget.hpp
  src/TurtleCanvasWidget.cpp
  src/TurtleAppWindow.hpp
  src/TurtleAppWindow.cpp
  src/TurtleApp.cpp
)

qt_add_executable(picoturtle-app MANUAL_FINALIZATION ${PICOTURTLE_APP_SOURCES})

target_link_libraries(picoturtle-app PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(picoturtle-app PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER abhishekmishra.in
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

set_property(TARGET picoturtle-app PROPERTY CXX_STANDARD 17)

add_subdirectory(argparse build)
include_directories(argparse)
target_link_libraries(picoturtle-app PRIVATE argparse)

# The output name of target should be "picoturtle"
# which generates picoturtle.exe on windows
set_target_properties(picoturtle-app PROPERTIES OUTPUT_NAME "picoturtle")

target_link_libraries(picoturtle-app PRIVATE picoturtle)

install(TARGETS picoturtle-app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

# see instructions at https://www.qt.io/blog/cmake-deployment-api
qt_generate_deploy_app_script(
    TARGET picoturtle-app
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

qt_finalize_executable(picoturtle-app)

# Run the qt deploy script for windows only
# TODO: need to check for release or Debug build to run different programs
if (WIN32)
  add_custom_command(
    TARGET picoturtle-app
    POST_BUILD
    COMMAND D:/vcpkg/installed/x64-windows/tools/Qt6/bin/windeployqt.debug.bat D:/code2/picoturtle/build/bin/Debug/picoturtle.exe
  )
endif(WIN32)

# Run the qt deploy script for windows only
# TODO: need to check for release or Debug build to run different programs
if (WIN32)
  install(CODE "execute_process(COMMAND D:/vcpkg/installed/x64-windows/tools/Qt6/bin/windeployqt.debug.bat D:/code2/picoturtle/install/bin/picoturtle.exe)")
else()
  install(SCRIPT ${deploy_script})
endif(WIN32)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
target_link_libraries(picoturtle-app 
  PRIVATE         
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)
target_link_libraries(picoturtle-app PRIVATE SDL2::SDL2_image)
target_link_libraries(picoturtle-app PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)
target_link_libraries(picoturtle-app PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)

# find_package(unofficial-graphicsmagick CONFIG REQUIRED)
# target_link_libraries(picoturtle-app PRIVATE unofficial::graphicsmagick::graphicsmagick)

