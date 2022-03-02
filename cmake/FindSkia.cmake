# Copyright (C) 2019-2021  Igara Studio S.A.
#
# This file is released under the terms of the MIT license.
# Read LICENSE.txt for more information.

set(SKIA_DIR "" CACHE PATH "Skia source code directory")
if(NOT SKIA_DIR)
  set(SKIA_LIBRARY_DIR "" CACHE PATH "Skia library directory (where libskia.a is located)")
else()
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SKIA_ARCH "x64")
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(SKIA_ARCH "x86")
  endif()
  if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(SKIA_LIBRARY_DIR "${SKIA_DIR}/out/Debug-${SKIA_ARCH}" CACHE PATH "Skia library directory")
  else()
    set(SKIA_LIBRARY_DIR "${SKIA_DIR}/out/Release-${SKIA_ARCH}" CACHE PATH "Skia library directory")
  endif()
endif()

# Skia library
find_library(SKIA_LIBRARY skia PATH "${SKIA_LIBRARY_DIR}")
if(WIN32)
  find_library(SKIA_OPENGL_LIBRARY opengl32)
elseif(APPLE)
  find_library(SKIA_OPENGL_LIBRARY OpenGL NAMES GL)
else()
  find_library(SKIA_OPENGL_LIBRARY opengl NAMES GL)
endif()

# SkShaper module + freetype + harfbuzz
find_library(SKSHAPER_LIBRARY skshaper PATH "${SKIA_LIBRARY_DIR}")

set(FREETYPE_FOUND ON)
find_library(FREETYPE_LIBRARY freetype2 PATH "${SKIA_LIBRARY_DIR}" NO_DEFAULT_PATH)
set(FREETYPE_LIBRARIES ${FREETYPE_LIBRARY})
set(FREETYPE_INCLUDE_DIRS "${SKIA_DIR}/third_party/externals/freetype/include")

find_library(HARFBUZZ_LIBRARY harfbuzz PATH "${SKIA_LIBRARY_DIR}" NO_DEFAULT_PATH)
set(HARFBUZZ_LIBRARIES ${HARFBUZZ_LIBRARY})
set(HARFBUZZ_INCLUDE_DIRS "${SKIA_DIR}/third_party/externals/harfbuzz/src")

set(SKIA_LIBRARIES
  ${SKIA_LIBRARY}
  ${SKIA_OPENGL_LIBRARY}
  CACHE INTERNAL "Skia libraries")

add_library(skia INTERFACE)
target_include_directories(skia INTERFACE
  ${SKIA_DIR}
  ${FREETYPE_INCLUDE_DIRS}
  ${HARFBUZZ_INCLUDE_DIRS})
target_link_libraries(skia INTERFACE ${SKIA_LIBRARIES})
target_compile_definitions(skia INTERFACE
  SK_INTERNAL
  SK_GAMMA_SRGB
  SK_GAMMA_APPLY_TO_A8
  SK_SCALAR_TO_FLOAT_EXCLUDED
  SK_ALLOW_STATIC_GLOBAL_INITIALIZERS=1
  SK_SUPPORT_OPENCL=0
  SK_FORCE_DISTANCE_FIELD_TEXT=0
  GR_GL_FUNCTION_TYPE=__stdcall
  SK_SUPPORT_GPU=0) # TODO change this to 1

if(WIN32)
  target_compile_definitions(skia INTERFACE
    SK_BUILD_FOR_WIN32
    _CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
  target_compile_definitions(skia INTERFACE
    SK_BUILD_FOR_MAC)
else()
  target_compile_definitions(skia INTERFACE
    SK_SAMPLES_FOR_X)
endif()

if(APPLE)
  find_library(COCOA_LIBRARY Cocoa)
  target_link_libraries(skia INTERFACE
    ${COCOA_LIBRARY})
endif()

if(UNIX AND NOT APPLE)
  # Change the kN32_SkColorType ordering to BGRA to work in X windows.
  target_compile_definitions(skia INTERFACE
    SK_R32_SHIFT=16)

  # Needed for SkFontMgr on Linux
  find_library(FONTCONFIG_LIBRARY fontconfig)
  target_link_libraries(skia INTERFACE
    ${FONTCONFIG_LIBRARY})
endif()

add_library(skshaper INTERFACE)
target_link_libraries(skshaper INTERFACE ${SKSHAPER_LIBRARY})
