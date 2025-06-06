# CHANGELOG This file documents major changes in every release of the project.
The project follows [Semantic Versioning](https://semver.org/). There is a
section for each release - which lists major changes made in the release.

**0.2.0-alpha.8**  2025-02-04 Abhishek Mishra  <abhishekmishra3@gmail.com>

- The 14th release of PicoTurtle incorporates some minor changes and bug fixes.
- The build system is now using a local vcpkg install instead of the global one.
  This is to ensure that the build is reproducible and does not depend on the
  global vcpkg install.
- The unused argparse module dependency is removed.
- There are changes to how the font is fetches, based on changes in Skia API.
- Some possible memory leaks were fixed.
- Windows packaging is fixed to include some missing dlls in the installer.

**0.2.0-alpha.7**  2024-01-12 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the 13th release of PicoTurtle. It's mostly a maintenance release.
- This release has updated to the latest version of Qt in vcpkg i.e. '6.6.1'.
- It also uses the latest skia version in vcpkg i.e. '0.38.2'.
- There are some breaking changes in the latest version skia, requiring some
  minor fixes. These fixes have only been tested on windows for now.  If there
  are more fixes required for other platforms I will make another release.
- The fixes relate to changes in SkCanvas class and in PNG encoding.

**0.2.0-alpha.6**  2023-02-12 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the 12th release of PicoTurtle. I've changed the cadence on release as
  changes are slowing down.
- This release contains several new turtle commands, new features and bug fixes.
- The documentation using `mkdocs` was completed sometime last week and went
  live at https://picoturtle.neolateral.in
- The color names lib `c-color-names` was updated to the latest release tag.
- The turtle API now contains two new commands `enable_update` and
  `disable_update` which enable and disable screen updates respectively. These
  commands when used with `paint` command (which forces screen update) can be
  used to build animations on the turtle canvas. I've developed a few examples
  which show how this can be used.
- A utility method `makegif` is added to the PicoTurtle lua module to enable
  creation of gifs from the snapshots taken from the turtle canvas. This can be
  used to build offline animations - when not using the on-screen canvas.
- The command `snap` is now an alias for `export_img` command.
- Two new commands `loadpic` and `pic` are added to the turtle API which enable
  loading of images from disk and drawing them on the turtle's location. More
  parameters will be added to the `pic` function to make it more powerful.
- A few new sample programs are added to the samples folder which use some of
  these new commands.
- The start location for the **Open File** dialog now uses the last opened
  file's location. This saves some time navigating to the same folder again and
  again.

**0.2.0-alpha.5**  2023-01-23 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the 11th weekly release of PicoTurtle. It incorporates a new
  **PicoTurtle CLI** program, and a brand new documentation site which is work
  in progress.
- There is a new program `ptcli` added to PicoTurtle which allows the user to
  run a PicoTurtle Lua program from the command line. This could be useful for
  automating some graphics generation. I've added this program because I want to
  generate the sample programs and images for the new documentation site in a
  bulk.
- The documentation site is added in the docs folder - and is built using
  [mkdocs](https://www.mkdocs.org/) and the [mkdocs Material
  theme](https://squidfunk.github.io/mkdocs-material/)
- The documentation is currently deployed using github actions. It is available
  at https://abhishekmishra.github.io/picoturtle. However the URL might change
  at a later date.


**0.2.0-alpha.4**  2023-01-15 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the 10th weekly release of PicoTurtle. It incorporates two major
  features in the Turtle lua code editor and some minor changes.
- The Turtle Lua Editor now has two edit action for **indent code** and
  **de-indent code** code. These have shortcuts assigned, and work like the
  usual indentation actions in other code editors.
- The editor also has a **toggle comment** action which comments (or uncomments)
  one or more lines using Lua "--" single line comments. Again these work
  similar to indentation actions in other code editor.
- The `Tab` key when pressed in the editor now adds a tab when there is no
  selection, and calls the **indent code** action when there is a selection.
- The `Shift-Tab` key always calls **de-indent** action whether there is a
  selection or not.
- The `c-color-names` version used in PicoTurtle is updated to the lates head
  version.
- There is a minor fix in documentation markdown.

**0.2.0-alpha.3**  2023-01-07 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the 9th weekly release of PicoTurtle and the first one in 2023. This
  release contains several documentation and installer related changed - and one
  major bug fix.
- The CMAKE build is used to generate an internal version header file - which
  can be used by other code to display the currect built version.
- The About Dialog now uses the version headers instead of hardcoding version.
- The CMAKE package section for windows uses the build version variables for
  generating installer strings.
- BugFix: When the last tab was closed in the IDE, the tab widget segfaults in
  the internal Qt handler. Essentially Qt does not support 0 tabs in the current
  version. So there is a workaround to add a new empty tab if the last editor is
  closed by the user.
- Documentation for `circle` and `arc` commands is added.

**0.2.0-alpha.2**  2022-12-31 Abhishek Mishra  <abhishekmishra3@gmail.com>

_Edit: I made an error while making the release tag - called it alpha.2. So this
release note is also amended accordingly._

- This is the 8th weekly release of PicoTurtle and the last one in 2022. This
  release contains a few minor features and bug fixes.
- The build targets for install and release have been changed to work with
  release binaries on Windows/Visual Studio.
- The installer build now contains release binaries instead of debug binaries.
  Since this was a relatively minor change, the binary for the previous version
  was also changed on the github release page.
- A new `circle(radius)` method is added to the `Turtle`. It draws a circle with
  the given radius around the current position of the turtle.
- A new `arc(radius, extent, steps)` based on the python turtle _circle_ method
  is added. This allows drawing of arcs, circles and regular polygons.
- The Canvas now supports a begin and end fill mode. This will be used to build
  fill capability in the turtle.

**0.2.0-alpha.0**  2022-12-24 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the 7th weekly release of PicoTurtle. I've bumped the minor version to
  2 - as starting this release there will be a windows installer available. The
  release contains the following changes.
- The mechanism of drawing the turtle (tiny red triangle indicating the turtle's
  position and heading) is moved to the canvas painter widget. This allows the
  gui to control when the turtle is drawn.
- Therefore the stop command is not longer explicitly needed in code - and does
  not do anything for now.
- Documentation is fully updated for all turtle commands in the docs/help.md
  file.
- `pw` and `pc` are new two letter aliases for `penwidth` and `pencolor` in the
  Turtle Lua API.
- The color-names dependency is upgraded to its **v0.1.0-alpha.0** tag - which
  contains the latest changes from upstream color-names package.
- The statusbar now has a separate display for showing current line number and
  column number of the editor.
- The CPack config to create a windows installer was added in last release -
  there are some minor changes to this section. This will be used to create the
  installer for windows and uploaded as part of this release.

**0.1.0-alpha.5**  2022-12-17 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the 6th alpha weekly release of PicoTurtle. This is perhaps the last
  source only release. It contains the following changes.
- The turtle `canvas_size` method now accepts either 2 arguments (to specify a
  new canvas size width X height) or 0 arguments. In both cases the current
  canvas size is returned as two values width, height.
- Minor fix in `BoxTurtle` class now passes the proper font arguments to the
  turtle `font` call.
- The `BoxTurtle` class now allows passing a color argument to the `clear`
  method. Thus the canvas can be cleared to any colour. This will later be added
  to the main turtle `clear` method as well.
- A new About dialog is added to PicoTurtle gui.
- The PicoTurtle docs are updated to use section numbers, and has a new turtle
  creation section.
- The docs are now displayed in a `QTextBrowser` for better readability.
- CPack is added to the cmake build. The windows installer creation is now
  working. A complete set of files will be added - and the windows installer
  will be part of the next release.

**0.1.0-alpha.4**  2022-12-10 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the fifth early alpha weekly release of PicoTurtle. It contains a
  couple of major features and a few bug fixes.
- The first major feature is *Turtle Lua Console Integration*. All turtle
  programs are now run as chunks in the lua repl widget. 
  - Every program run restarts the lua repl session 
  - This enables the reload of any dependent lua modules which might have
    changed.
  - The use of docall to load the chunk implies that the global variables are
    available for inspection post the run.
  - The turtle object is created by the repl class in global scope. This allows
    the inspection of turtle object in the repl post-run. It also allows user to
    issue turtle commands via the repl.
- The second major feature is "Find/replace widget". A naive implementation
  without current support for regex or match/ignore case is implemented. These
  features will be added in the coming releases.
- Bug fix: if open file dialog was cancelled, an empty tab was added to editor.
  This is now fixed - no tab is added when the action is cancelled.
- The menu editor commands are now connected to corresponding actions in the
  editor widget.
- If the lua file being run is saved on disk (has a valid path) then the
  directory containing the file is added to the lua path. Thus all require
  commands can resolve modules in that directory. This is helpful for running
  multi-file projects.

**0.1.0-alpha.3**  2022-12-04 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the fourth early alpha weekly release of PicoTurtle. It contains
  several minor changes/fixes and one major feature.
- The fern drawing sample program in the README.md file is updated.
- The Box and BoxTurtle lua classes now optionally check the bounds for each
  movement of the turtle. The option is supplied in the constructor. If
  check_bounds is enabled the BoxTurtle keeps the turtle moving only inside the
  box. Any move taking the turtle outside the box is invalid and has no effect.
- To enable gameloop like mechanics for some turtle programs the following
  methods are added to the turtle API:

  1. `elapsed_time_ms` - Gives the total time in millis since the turtle was
  constructed.
  2. `paint` - Explicitly calls the paint callback if specified. This should be
  used to paint the turtle on the UI canvas.
  3. `delay` - Add an explicit delay to the turtle to slow it down. This calls
  the delay callback if specified. In non-interactive runs this method can be
  ignored - by not providing a callback to implement it.
  4. `update` - A function which calls the update callback whenever the state of
  the turtle changes. This helps the UI take some actions when the turtle is
  modified.
- The major feature added in this release is the Lua Repl Widget in the UI. This
  widget is based on the code in lua.c of lua5.4 and implements an interactive
  lua console. Currently the turtle is not enabled in this console. In the next
  release turtle launches and interaction will be added to this widget.

**0.1.0-alpha.2**  2022-11-26 Abhishek Mishra  <abhishekmishra3@gmail.com>

- Third alpha release of PicoTurtle with several features and bug fixes.
- Box and BoxTurtle lua modules now implement padding, margin, and border - and
  allow the turtle to draw in the inner box. BoxTurtle can also draw a border.
- Added Vec4 to support specifying padding, margin and border values.
- Included c-color-names in picoturtle-lib so that the pencolor method can
  support specifying color as a string name. Old way of specifying rgb values is
  also supported.
- All _pencolour_ methods are renamed *pencolor* to follow american spelling
  which is standard in the programming world.
- Since a lot of the turtle code was based on an older C# version of the code -
  it was in sentence case. These methods have all been changed to lowercase with
  underscore to separate words.
- The public turtle lua api now supports state(), save(), and restore() - to get
  the turtle state, save the current state, and restore the saved state. This
  allows to temporarily switch turtle properties, do some action and resume from
  where the state was saved.
- Fixed the color type and alpha type extraction from SkImage generated by the
  SkCanvas snapshot - when drawing on the GL Widget. Earlier the color detection
  was based on Skia preprocessor macros which gave incorrect results on macros.
  Now the color type is detected on the first turtle run and reused for later
  calls.
- Antialiasing is set on skia and on the gl widget painter to enable much better
  and smoother image output.
- Min width of the turtle canvas is set, and the canvas widget is now in a dock
  tabbed along with the turtle docs widget.
- The turtle canvas dock is raised on a turtle run and the docs dock is raised
  when opening docs.
- Text rotation is fixed in SkiaCanvas class to rotate around the pivot of the
  current turtle position.
- The text demo in samples is also appropriately fixed.
- The maze generation sample is fixed to work with the current api.

**0.1.0-alpha.1**  2022-11-19 Abhishek Mishra  <abhishekmishra3@gmail.com>

- Second early alpha release of the new PicoTurtle, many minor and major
  features added
- A sample turtle lua program is opened in a new editor when the program starts
- The colours of the console and editor are based on Turbo Pascal IDE colours.
- The turtle documentation is copied over from previous PicoTurtle and shown in
  a new dock - for starters.
- Using the system fixed font (discovered via Qt apis) for the editor and
  console.
- Added icons for both dark and light version. Using helper function to detect
  dark system them and switch icons to white.
- Save button is enabled/disabled based on whether the editor is dirty.
- Editor tab width is set to 4 spaces.
- Editor now has syntax highlighting according to lua syntax.
- Program has icon (copied from previous PicoTurtle) for windows and macos
  versions.
- The picoturtle library now has separation of concerns between turtle and
  canvas - which now have separate class heirarchies of interface and
  implementation.
- The picoturtle canvas object is now a static instance and shared among
  multiple instantiated turtle objects
- A set of lua modules providing a BoxTurtle (which restricts a turtle instance
  to drawing in a box) are added.
- The BoxTurtle and picoturtle lib changes enable drawing multiple turtle
  experiments on the same canvas.
- The print function embedded into lua runtime now uses luaL_tolstring so that
  proper string conversion of non-primitive types is done for printing.
- The gl canvas for drawing the turtle canvas now resizes, alongwith the window
  and stays squared and centered.
- Provision for loading a lua config file from standard platform config location
  is added, but not used in this release.

**0.1.0-alpha.0**  2022-11-12 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the first early alpha release of the full rewrite of PicoTurtle in
  C/C++/Lua.
- Note that there was an earlier prototype version of PicoTurtle in NodeJS and
  electron. It supported multiple programming languages. However both
  performance and ease of development was not satisfactory.
- This project is developed using C++ mostly (with some parts interacting with
  Lua written in C). The turtle is currently drawn on a Skia canvas.
- Turtle programs can be written in Lua.
- It's also theoretically possible to use the turtle library and write the
  turtle programs in C++.
- The GUI is developed using Qt6 - and has a basic editor and canvas to play
  around with a turtle.
- The current release is under GPLv3. At a later date the library component may
  be released under LGPLv3.
