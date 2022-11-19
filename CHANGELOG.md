# CHANGELOG
This file documents major changes in every release of the project. The project follows [Semantic Versioning](https://semver.org/). There is a section for each release - which lists major changes made in the release.

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