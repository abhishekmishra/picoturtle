---
date: 25/02/2024
author: Abhishek Mishra
---

# PicoTurtle Lua Files

This file is currently in a directory called "lua". It is currently home to
lua code which is part of the PicoTurtle source, but is not shipped in the
installer.

Here are the files/modules currently available:
1. **mazes**: An extended example lua program showing features of PicoTurtle,
   based on an old published article.
2. **samples/**: A folder containing simple turtle graphics programs 
   demonstrating various features of PicoTurtle.
3. **middleclass.lua**: A library used to create Classes. This is used by the
   modules in `pt/` folder.
4. **pt/**: Package containing various lua util programs which implment
   `Box`. A `Box` is a rectangular area in the Turtle Canvas, which has all
   the methods of the `Turtle`. This allows us to define multiple canvases in
   the same canvas, and also to define canvases inside canvases recursively.

# Issues/TODOs

This folder has grown organically and therefore is not organized.

TODO: Create an organization plan for this folder.

TODO: Create a package naming structure for current and future additions to this
folder.

TODO: Organize the current modules in this folder according to plan.

TODO: Modify any programs dependent on the modules in this folder to use the
new organization.

TODO: Add documentation for all the modules in the folder.

TODO: Create a jsdoc document generation step in the build.

TODO: Show the generated documentation in the PicoTurtle IDE.

TODO: Deploy the code and documentation in this folder to the final installer.

TODO: Write test cases for the modules in this folder.