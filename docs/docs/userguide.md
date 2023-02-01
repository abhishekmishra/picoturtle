# PicoTurtle User Guide

The PicoTurtle Desktop Application is a simple and intuitive interface.
Therefore it doesn't need any long explanations. Here I present a high-level
overview of how the application is organized - and what each part does.

I urge that you take a quick look at this document and then try things out
yourself. If there are any doubts or questions you can always come back to it
and refer to the relevant area.

## PicoTurtle App

When you launch the PicoTurtle desktop application on your system you are
presented with a single window. This window already shows most of the parts
of the app.

### PicoTurtle at Launch

![PicoTurtle At Launch](images/pt-ss-default-win11-v0_2_0-empty.png "PicoTurtle At Launch")

### Overview

With the newly launched PicoTurtle open, if you hit the key "F5" or click the 
button which looks like the play button (with a tiny triangle), you will run 
the default turtle program and the code will draw some lines and text on the 
canvas.

The PicoTurtle application has the following key parts which are highlighted
in the next screenshot:

1. **Titlebar**: shows the currently open file. An asterix indicates if the
   currently open file has unsaved changes.
2. **Menubar**: Provides access to all the application functions via menus and
   nested sub-menus.
3. **Toolbar**: Provides access to some frequently used functions via toolbar
   buttons.
4. **Turtle Lua Code Editor**: You will spend most of your time here - editing
   turtle commands and code.
5. **Turtle Canvas**: When a turtle program is run, it draws on a canvas. The
   canvas is displayed in this panel to the right of the window.
6. **Lua Console**: Sometimes you want to send a temporary command to the app,
   but do not want to write it in the code editor. You can do this here. This
   console also displays any textual output from the code.
7. **Statusbar**: A few informative items about the application are displayed
   here. For e.g. it shows the current line and column of the cursor in the
   code editor.

![PicoTurtle First Run](images/pt-ss-default-win11-v0_2_0-annotated.png "PicoTurtle First Run")

In the following sections the major components will be described in a bit
more detail.

### The Lua Turtle Editor

The Turtle Editor is a simple text editor which supports editing Lua code.
It has the following features:

1. Lua Syntax Highlighting
2. Line Numbers
3. Multiple Tabs (for multiple open files)
4. Docked Window (can maximize and move)
5. Indent and De-indent (using shortcuts `Tab` and `Shift-Tab`)
6. Comment and Un-comment (using toggle shortcut `Ctrl-/`)

![Lua Editor](images/pt-ss-editor-sun-macos-v0_2_0.png "Lua Editor")

### The Turtle Canvas

The Turtle Canvas is one of the simplest widgets - it just displays the output
of the current turtle program when run. It is also a Docked window.

![Turtle Canvas](images/pt-ss-canvas-sun-macos-v0_2_0.png "Turtle Canvas")

### Toolbar and Menubar

The Menubar provides access to **ALL** PicoTurtle functions. Whereas the
Toolbar provides access to a few frequently used functions.

The **Edit Menu** looks like this... The commands which have a shortcut,
have the shortcut code displayed alongside their names.

![Edit Menu](images/pt-ss-editmenu-macos-v0_2_0.png "Edit Menu")

### Lua Interactive Console

* The Lua Console is a window where you might interact with the current 
  *Turtle*.
* You can write Lua statements in the text entry at the bottom of the console.
* Once you press `Enter`, the Lua statement is sent to the interal Lua runtime.
* The statement followed by its results are shown in the multi-line text
  display above the text entry.
* If the statement is a Turtle command, then the changes should reflect in the
  canvas.

![Lua Console](images/pt-ss-luaconsole-macos-v0_2_0.png "Lua Console")

### Statusbar

The statusbar is at the bottom of the window and shows a few interesting
information or error messages.


## PicoTurtle CLI

TBD

## PicoTurtle Library

### C Library Usage

TBD

### Lua Module Usage

```lua
local picoturtle = require'picoturtle'
local t = picoturtle.new()
```