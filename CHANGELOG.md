# CHANGELOG
This file documents major changes in every release of the project. The project follows [Semantic Versioning](https://semver.org/). There is a section for each release - which lists major changes made in the release.

**0.2.0-alpha.0**  2022-12-24 Abhishek Mishra  <abhishekmishra3@gmail.com>
- This is the 7th weekly release of PicoTurtle. I've bumped the minor version to 2 - as starting this release there will be a windows installer available. The release contains the following changes.
- The mechanism of drawing the turtle (tiny red triangle indicating the turtle's position and heading) is moved to the canvas painter widget. This allows the gui to control when the turtle is drawn.
- Therefore the stop command is not longer explicitly needed in code - and does not do anything for now.
- Documentation is fully updated for all turtle commands in the docs/help.md file.
- `pw` and `pc` are new two letter aliases for `penwidth` and `pencolor` in the Turtle Lua API.
- The color-names dependency is upgraded to its **v0.1.0-aplha.0** tag - which contains the latest changes from upstream color-names package.
- The statusbar now has a separate display for showing current line number and column number of the editor.
- The CPack config to create a windows installer was added in last release - there are some minor changes to this section. This will be used to create the installer for windows and uploaded as part of this release.

**0.1.0-alpha.5**  2022-12-17 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the 6th alpha weekly release of PicoTurtle. This is perhaps the last source only release. It contains the following changes.
- The turtle `canvas_size` method now accepts either 2 arguments (to specify a new canvas size width X height) or 0 arguments. In both cases the current canvas size is returned as two values width, height.
- Minor fix in `BoxTurtle` class now passes the proper font arguments to the turtle `font` call.
- The `BoxTurtle` class now allows passing a color argument to the `clear` method. Thus the canvas can be cleared to any colour. This will later be added to the main turtle `clear` method as well.
- A new About dialog is added to PicoTurtle gui.
- The PicoTurtle docs are updated to use section numbers, and has a new turtle creation section.
- The docs are now displayed in a `QTextBrowser` for better readability.
- CPack is added to the cmake build. The windows installer creation is now working. A complete set of files will be added - and the windows installer will be part of the next release.

**0.1.0-alpha.4**  2022-12-10 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the fifth early alpha weekly release of PicoTurtle. It contains a couple of major features and a few bug fixes.
- The first major feature is *Turtle Lua Console Integration*. All turtle programs are now run as chunks in the lua repl widget. 
  - Every program run restarts the lua repl session 
  - This enables the reload of any dependent lua modules which might have changed.
  - The use of docall to load the chunk implies that the global variables are available for inspection post the run.
  - The turtle object is created by the repl class in global scope. This allows the inspection of turtle object in the repl post-run. It also allows user to issue turtle commands via the repl.
- The second major feature is "Find/replace widget". A naive implementation without current support for regex or match/ignore case is implemented. These features will be added in the coming releases.
- Bug fix: if open file dialog was cancelled, an empty tab was added to editor. This is now fixed - no tab is added when the action is cancelled.
- The menu editor commands are now connected to corresponding actions in the editor widget.
- If the lua file being run is saved on disk (has a valid path) then the directory containing the file is added to the lua path. Thus all require commands can resolve modules in that directory. This is helpful for running multi-file projects.

**0.1.0-alpha.3**  2022-12-04 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the fourth early alpha weekly release of PicoTurtle. It contains several minor changes/fixes and one major feature.
- The fern drawing sample program in the README.md file is updated.
- The Box and BoxTurtle lua classes now optionally check the bounds for each movement of the turtle. The option is supplied in the constructor. If check_bounds is enabled the BoxTurtle keeps the turtle moving only inside the box. Any move taking the turtle outside the box is invalid and has no effect.
- To enable gameloop like mechanics for some turtle programs the following methods are added to the turtle API:

  1. `elapsed_time_ms` - Gives the total time in millis since the turtle was constructed.
  2. `paint` - Explicitly calls the paint callback if specified. This should be used to paint the turtle on the UI canvas.
  3. `delay` - Add an explicit delay to the turtle to slow it down. This calls the delay callback if specified. In non-interactive runs this method can be ignored - by not providing a callback to implement it.
  4. `update` - A function which calls the update callback whenever the state of the turtle changes. This helps the UI take some actions when the turtle is modified.
- The major feature added in this release is the Lua Repl Widget in the UI. This widget is based on the code in lua.c of lua5.4 and implements an interactive lua console. Currently the turtle is not enabled in this console. In the next release turtle launches and interaction will be added to this widget.

**0.1.0-alpha.2**  2022-11-26 Abhishek Mishra  <abhishekmishra3@gmail.com>

- Third alpha release of PicoTurtle with several features and bug fixes.
- Box and BoxTurtle lua modules now implement padding, margin, and border - and allow the turtle to draw in the inner box. BoxTurtle can also draw a border.
- Added Vec4 to support specifying padding, margin and border values.
- Included c-color-names in picoturtle-lib so that the pencolor method can support specifying color as a string name. Old way of specifying rgb values is also supported.
- All _pencolour_ methods are renamed *pencolor* to follow american spelling which is standard in the programming world.
- Since a lot of the turtle code was based on an older C# version of the code - it was in sentence case. These methods have all been changed to lowercase with underscore to separate words.
- The public turtle lua api now supports state(), save(), and restore() - to get the turtle state, save the current state, and restore the saved state. This allows to temporarily switch turtle properties, do some action and resume from where the state was saved.
- Fixed the color type and alpha type extraction from SkImage generated by the SkCanvas snapshot - when drawing on the GL Widget. Earlier the color detection was based on Skia preprocessor macros which gave incorrect results on macros. Now the color type is detected on the first turtle run and reused for later calls.
- Antialiasing is set on skia and on the gl widget painter to enable much better and smoother image output.
- Min width of the turtle canvas is set, and the canvas widget is now in a dock tabbed along with the turtle docs widget.
- The turtle canvas dock is raised on a turtle run and the docs dock is raised when opening docs.
- Text rotation is fixed in SkiaCanvas class to rotate around the pivot of the current turtle position.
- The text demo in samples is also appropriately fixed.
- The maze generation sample is fixed to work with the current api.

**0.1.0-alpha.1**  2022-11-19 Abhishek Mishra  <abhishekmishra3@gmail.com>

- Second early alpha release of the new PicoTurtle, many minor and major features added
- A sample turtle lua program is opened in a new editor when the program starts
- The colours of the console and editor are based on Turbo Pascal IDE colours.
- The turtle documentation is copied over from previous PicoTurtle and shown in a new dock - for starters.
- Using the system fixed font (discovered via Qt apis) for the editor and console.
- Added icons for both dark and light version. Using helper function to detect dark system them and switch icons to white.
- Save button is enabled/disabled based on whether the editor is dirty.
- Editor tab width is set to 4 spaces.
- Editor now has syntax highlighting according to lua syntax.
- Program has icon (copied from previous PicoTurtle) for windows and macos versions.
- The picoturtle library now has separation of concerns between turtle and canvas - which now have separate class heirarchies of interface and implementation.
- The picoturtle canvas object is now a static instance and shared among multiple instantiated turtle objects
- A set of lua modules providing a BoxTurtle (which restricts a turtle instance to drawing in a box) are added.
- The BoxTurtle and picoturtle lib changes enable drawing multiple turtle experiments on the same canvas.
- The print function embedded into lua runtime now uses luaL_tolstring so that proper string conversion of non-primitive types is done for printing.
- The gl canvas for drawing the turtle canvas now resizes, alongwith the window and stays squared and centered.
- Provision for loading a lua config file from standard platform config location is added, but not used in this release.

**0.1.0-alpha.0**  2022-11-12 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the first early alpha release of the full rewrite of PicoTurtle in C/C++/Lua.
- Note that there was an earlier prototype version of PicoTurtle in NodeJS and electron. It supported multiple programming languages. However both performance and ease of development was not satisfactory.
- This project is developed using C++ mostly (with some parts interacting with Lua written in C). The turtle is currently drawn on a Skia canvas.
- Turtle programs can be written in Lua.
- It's also theoretically possible to use the turtle library and write the turtle programs in C++.
- The GUI is developed using Qt6 - and has a basic editor and canvas to play around with a turtle.
- The current release is under GPLv3. At a later date the library component may be released under LGPLv3.