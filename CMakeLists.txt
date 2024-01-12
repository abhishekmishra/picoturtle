cmake_minimum_required ( VERSION 3.20 )

project ( picoturtle VERSION 0.2.0 LANGUAGES C CXX )

set (VERBOSE)

# Export all symbols for shared linking to work on windows
# see https://stackoverflow.com/questions/37128555/getting-cmake-to-build-shared-library-for-msvc
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

set( CMAKE_CXX_FLAGS " -pthread " )

# Search in the "cmake" directory for the additional CMake modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# It's useful to setup the current version of our code in the build system
# using a `semver` style
set ( PICOTURTLE_VERSION_MAJOR "0" )
set ( PICOTURTLE_VERSION_MINOR "2" )
set ( PICOTURTLE_VERSION_PATCH "0" )
set ( PICOTURTLE_VERSION_PRERELEASE "-alpha.7" )
set ( PICOTURTLE_VERSION_STR ${PICOTURTLE_VERSION_MAJOR}.${PICOTURTLE_VERSION_MINOR}.${PICOTURTLE_VERSION_PATCH}${PICOTURTLE_VERSION_PRERELEASE})

# ----------- Configure Version File
configure_file( src/PicoTurtleVersion.hpp.in src/PicoTurtleVersion.hpp )

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
  lua/pt/Box.lua
  lua/pt/Vec2.lua
  lua/pt/Vec4.lua
  lua/pt/BoxTurtle.lua
  lua/pt/BoxUtils.lua
  lua/ptall.lua
  lua/middleclass.lua
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
  c-color-names/color_names.h
  c-color-names/color_names.c
  src/GifUtil.h
  src/GifUtil.c
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
  src/Canvas.hpp
  src/Canvas.cpp
  src/SkiaCanvas.hpp
  src/SkiaCanvas.cpp
  src/Turtle.hpp
  src/Turtle.cpp
  src/PicoTurtle.hpp
  src/PicoTurtle.cpp
  src/PicoTurtleLua.hpp
  src/PicoTurtleLua.cpp
  src/PicoTurtleImage.hpp
  src/PicoTurtleImage.cpp
)

add_library(picoturtle SHARED ${PICOTURTLE_LIB_SOURCES})
set_property(TARGET picoturtle PROPERTY CXX_STANDARD 17)

target_include_directories(picoturtle PUBLIC c-color-names)

target_include_directories(picoturtle PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/src)

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

find_package(unofficial-graphicsmagick CONFIG REQUIRED)
target_link_libraries(picoturtle PUBLIC 
  unofficial::graphicsmagick::graphicsmagick)

#--------------------------PicoTurtle CLI---------------------------#

set ( PICOTURTLE_CLI_SOURCES
    src/PicoTurtleCli.cpp
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable( ptcli ${PICOTURTLE_CLI_SOURCES} )
target_link_libraries( ptcli PUBLIC picoturtle )

find_package(coll CONFIG REQUIRED)
target_link_libraries(ptcli PUBLIC coll::coll)

find_package(zclk CONFIG REQUIRED)
target_link_libraries(ptcli PUBLIC zclk::zclk)

find_package(Lua REQUIRED)
target_include_directories(ptcli PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(ptcli PRIVATE ${LUA_LIBRARIES})

#--------------------------PicoTurtle App---------------------------#

# see https://doc.qt.io/qt-6/resources.html for resource packaging.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets)

# Setting up icons in windows and macos.

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/res/picoturtle.rc")


# The MACOSX_BUNDLE_ICON_FILE variable is added to the Info.plist
# generated by CMake. This variable contains the .icns file name,
# without the path.
set(MACOSX_BUNDLE_ICON_FILE pico.icns)

# And the following tells CMake where to find and install the file itself.
set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/res/icons/pico.icns")
set_source_files_properties(${app_icon_macos} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")

set ( PICOTURTLE_APP_SOURCES
    res/picoturtle.qrc
    src/TurtleGLWidget.hpp
    src/TurtleGLWidget.cpp
    src/LuaSyntaxHighlighter.hpp
    src/LuaSyntaxHighlighter.cpp
    src/TurtleFindReplaceTextWidget.hpp
    src/TurtleFindReplaceTextWidget.cpp
    src/TurtleLineNumberAreaWidget.hpp
    src/TurtleLineNumberAreaWidget.cpp
    src/TurtleCodeEditorTextWidget.hpp
    src/TurtleCodeEditorTextWidget.cpp
    src/TurtleCodeEditorWidget.hpp
    src/TurtleCodeEditorWidget.cpp
    src/TurtleCodeEditorParentWidget.hpp
    src/TurtleCodeEditorParentWidget.cpp
    src/LuaReplWidget.hpp
    src/LuaReplWidget.cpp
    src/TurtleLuaReplWidget.hpp
    src/TurtleLuaReplWidget.cpp
    src/TurtleConsoleWidget.hpp
    src/TurtleConsoleWidget.cpp
    src/TurtleCanvasWidget.hpp
    src/TurtleCanvasWidget.cpp
    src/TurtleDocsWidget.hpp
    src/TurtleDocsWidget.cpp
    src/TurtleAboutDialog.hpp
    src/TurtleAboutDialog.cpp
    src/TurtleAppWindow.hpp
    src/TurtleAppWindow.cpp
    src/TurtleApp.cpp
    ${app_icon_macos}
    ${app_icon_resource_windows}
)

qt_add_executable(picoturtle-app MANUAL_FINALIZATION ${PICOTURTLE_APP_SOURCES})

target_link_libraries(picoturtle-app PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

find_package(Qt6OpenGL CONFIG REQUIRED)
target_link_libraries(picoturtle-app PRIVATE Qt::OpenGL Qt6::OpenGL Qt::OpenGLPrivate Qt6::OpenGLPrivate)

find_package(Qt6Widgets CONFIG REQUIRED)
# note: 1 additional targets are not displayed.
target_link_libraries(picoturtle-app PRIVATE Qt::Widgets Qt6::Widgets Qt::WidgetsPrivate Qt6::WidgetsPrivate)

find_package(Qt6OpenGLWidgets CONFIG REQUIRED)
target_link_libraries(picoturtle-app PRIVATE Qt::OpenGLWidgets Qt6::OpenGLWidgets Qt::OpenGLWidgetsPrivate Qt6::OpenGLWidgetsPrivate)

set_target_properties(picoturtle-app PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER abhishekmishra.in
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

set_property(TARGET picoturtle-app PROPERTY CXX_STANDARD 17)

# TODO: removed argparse for now from gui app
# TODO: will add argument parsing in turtle console app
# add_subdirectory(argparse build)
# include_directories(argparse)
# target_link_libraries(picoturtle-app PRIVATE argparse)

# The output name of target should be "picoturtle"
# which generates picoturtle.exe on windows
set_target_properties(
  picoturtle-app 
  PROPERTIES OUTPUT_NAME "picoturtle"
  FRAMEWORK FALSE
)

target_link_libraries(picoturtle-app PUBLIC picoturtle)

install(TARGETS picoturtle-app picoturtle
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_BINDIR})

# ----- Install all the dependencies of the application to the bin folder ------
# ----- (exclude windows runtime dependencies (handled separately))       ------
install(TARGETS picoturtle-app
    RUNTIME_DEPENDENCIES
    PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
    POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION .
)

# see instructions at https://www.qt.io/blog/cmake-deployment-api
qt_generate_deploy_app_script(
    TARGET picoturtle-app
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

qt_finalize_executable(picoturtle-app)

# Generate docs before the build.
add_custom_command(
    OUTPUT ../res/docs/help.html
    #TARGET picoturtle-app
    #PRE_BUILD
    COMMAND pandoc -s --embed-resources --toc --number-sections -o res/docs/help.html --css=docs/pandoc.css docs/docs/api.md
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )

# Run the qt deploy script for windows only
# This command uses generator expressions to figure out which windeployqt command to use
# if $<CONFIG:Debug> is 1 it uses the debug extension,
# if $<CONFIG:Release> is 1 it uses the exe.
# similar logic is used to choose the argument of the command
# which should be the executable built for this config
if (WIN32)
  add_custom_command(
    TARGET picoturtle-app
    POST_BUILD
    COMMAND D:/vcpkg/installed/x64-windows/tools/Qt6/bin/windeployqt.$<$<CONFIG:Debug>:debug.bat>$<$<CONFIG:Release>:exe> D:/code2/picoturtle/build/bin/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/picoturtle.exe
  )
endif(WIN32)

# Run the qt deploy script for windows only
# TODO: need to check for release or Debug build to run different programs
if (WIN32)
  if($<CONFIG:Debug>)
    install(CODE "execute_process(COMMAND D:/vcpkg/installed/x64-windows/tools/Qt6/bin/windeployqt.debug.bat D:/code2/picoturtle/install/bin/picoturtle.exe)")
  endif()
else()
  install(SCRIPT ${deploy_script})
endif(WIN32)

# ----- License file ------
install(
  FILES 
  ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
  ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE-Lua
  ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE-Qt
  ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE-Skia
  DESTINATION .
)

# ----- Packaging all the files generated by deployqt command ------
# ----- (these are not automatically detected as dependencies) -----
if (WIN32)
  install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/imageformats DESTINATION bin)
  install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/translations DESTINATION bin)
  install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/platforms DESTINATION bin)
  install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/styles DESTINATION bin)
endif(WIN32)

# ----- Windows specific NSIS settings ------
if (WIN32)
  set (CPACK_NSIS_DISPLAY_NAME "PicoTurtle v${PICOTURTLE_VERSION_STR}")
  set (CPACK_NSIS_PACKAGE_NAME "PicoTurtle")

  set(CPACK_NSIS_HELP_LINK "https://github.com/abhishekmishra/picoturtle")
  set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/abhishekmishra/picoturtle")
  set(CPACK_NSIS_CONTACT "https://github.com/abhishekmishra/picoturtle")

  set(CPACK_NSIS_WELCOME_TITLE "Welcome to PicoTurtle Setup.")
  set(CPACK_NSIS_FINISH_TITLE "Congrats! You've successfully installed PicoTurtle!")
  set(CPACK_NSIS_BRANDING_TEXT "PicoTurtle v${PICOTURTLE_VERSION_STR} by Abhishek Mishra")

  # include windows runtime libraries
  include (InstallRequiredSystemLibraries)

  # set the install/uninstall icon used for the installer itself
  set (CPACK_NSIS_MUI_ICON
      "${CMAKE_CURRENT_SOURCE_DIR}/res/icons/pico.ico")
  set (CPACK_NSIS_MUI_UNIICON
      "${CMAKE_CURRENT_SOURCE_DIR}/res/icons/pico.ico")
  
  # set the add/remove programs icon using an installed executable
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin/picoturtle.exe")

  # set the image displayed on the top left of installer screen
  #set (CPACK_PACKAGE_ICON
  #   "${CMAKE_CURRENT_SOURCE_DIR}/res/icons/pico.bmp")

  # License displayed in installer
  set (CPACK_RESOURCE_FILE_LICENSE
     "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

  # Create desktop links
  set (CPACK_CREATE_DESKTOP_LINKS picoturtle)

  # Set start menu links
  set (CPACK_PACKAGE_EXECUTABLES "picoturtle" "PicoTurtle" )

  # TODO: use this to create additional links to docs, license etc.
  #set (CPACK_NSIS_MENU_LINKS
  #   "doc/cmake-${VERSION_MAJOR}.${VERSION_MINOR}/cmake-gui.html"
  #   "cmake-gui Help" "http://www.cmake.org" "CMake Web Site")

endif(WIN32)

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    message ( "***** Setting CPack Generator Options For Linux *****" )
    set ( CPACK_BINARY_DEB ON )
    set ( CPACK_BINARY_STGZ OFF )
    set ( CPACK_BINARY_TGZ OFF )
    set ( CPACK_BINARY_TZ OFF )
    
    set ( CPACK_DEBIAN_PACKAGE_MAINTAINER "Abhishek Mishra" )
endif()

include(CPack)
