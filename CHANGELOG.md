# CHANGELOG
This file documents major changes in every release of the project. The project follows [Semantic Versioning](https://semver.org/). There is a section for each release - which lists major changes made in the release.

**0.1.0-alpha.0**  2022-11-12 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the first early alpha release of the full rewrite of PicoTurtle in C/C++/Lua.
- Note that there was an earlier prototype version of PicoTurtle in NodeJS and electron. It supported multiple programming languages. However both performance and ease of development was not satisfactory.
- This project is developed using C++ mostly (with some parts interacting with Lua written in C). The turtle is currently drawn on a Skia canvas.
- Turtle programs can be written in Lua.
- It's also theoretically possible to use the turtle library and write the turtle programs in C++.
- The GUI is developed using Qt6 - and has a basic editor and canvas to play around with a turtle.
- The current release is under GPLv3. At a later date the library component may be released under LGPLv3.