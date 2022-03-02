cmake_minimum_required ( VERSION 3.9 )

project ( picoturtle LANGUAGES CXX )

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

#--------------------------PicoTurtle App---------------------------#

set ( PICOTURTLE_SOURCES
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
  src/SkiaTurtle.hpp
  src/SkiaTurtle.cpp
  src/SkiaTurtleLua.hpp
  src/SkiaTurtleLua.cpp
  src/TurtleApp.cpp
)

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

add_executable(picoturtle ${PICOTURTLE_SOURCES})
set_property(TARGET picoturtle PROPERTY CXX_STANDARD 17)

if(WIN32)
  if (NOT TARGET skia)
    find_package(skia CONFIG REQUIRED)
  endif()
  target_include_directories(skia INTERFACE "$ENV{VCPKG_ROOT}/installed/x64-windows/include/skia")
	target_link_libraries(picoturtle PRIVATE skia skia::skia)
endif(WIN32)

if(UNIX)
  set(OpenGL_GL_PREFERENCE GLVND)
  find_package(OpenGL)

  link_directories(/usr/lib/x86_64-linux-gnu/)

  target_link_libraries(picoturtle PUBLIC OpenGL::OpenGL OpenGL::GLX GLX OpenGL)
  if (NOT TARGET skia)
  	include(FindSkia)
    target_include_directories(skia INTERFACE
      ${SKIA_DIR}/include
    )
    target_link_libraries(picoturtle PUBLIC skia)
  endif()
endif(UNIX)

find_package(Lua REQUIRED)
target_include_directories(picoturtle PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(picoturtle PUBLIC ${LUA_LIBRARIES})

find_package(unofficial-graphicsmagick CONFIG REQUIRED)
target_link_libraries(picoturtle PRIVATE unofficial::graphicsmagick::graphicsmagick)