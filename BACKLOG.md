# 0.3.0 (matrix-goose)

## Release goal

Replace the Skia/Qt implementation with a focused Raylib-based PicoTurtle
runtime that supports the established public Lua graphics API without carrying
forward the IDE, editor, or Qt application.

The release should run PicoTurtle Lua programs from a user's preferred editor,
display their output in a responsive Raylib window, and support image export.

All items below are in scope for this release. Before tagging the release, each
item must either be completed or deliberately moved to a later release backlog.

## Definition of done

- The Raylib runtime builds reproducibly on macOS through the root Makefile
  and vcpkg setup.
- Existing PicoTurtle Lua programs can run without depending on Qt or Skia.
- Every public Lua method from the original PicoTurtle implementation is either
  supported or explicitly documented as deprecated with a replacement.
- The original sample suite is used as the compatibility and regression suite.
- Static drawings can be exported to PNG.
- Animated programs remain responsive while painting or delaying.
- Core state and movement behaviour is covered by automated tests.

## Milestones

- [x] Milestone 1: Establish a sound Raylib runtime and canvas
  - [x] Add a runtime/canvas structure that owns the Raylib window and
        persistent `RenderTexture2D`.
  - [x] Associate each turtle with the shared runtime canvas.
  - [x] Initialize Raylib and the canvas before initializing and executing Lua.
  - [x] Route all drawing operations through valid `BeginTextureMode` and
        `EndTextureMode` scopes.
  - [x] Present the persistent canvas texture in a responsive window loop.
  - [x] Define clean shutdown and release all Raylib resources.
  - [x] Support multiple turtles drawing on the shared canvas.
  - [x] Decide and document normal windowed and non-interactive export
        behaviour.
  - [x] Remove the experimental drawing harness from the production execution
        path.

- [x] Milestone 2: Restore core turtle and Lua API compatibility
  - [x] Rewrite Lua bindings to read turtle userdata from stack index 1 and
        method arguments from subsequent indices.
  - [x] Add consistent Lua argument-count, type, and range validation.
  - [x] Restore the original logical coordinate system, including Y-axis
        conversion between turtle and Raylib coordinates.
  - [x] Restore the initial position at the centre of the canvas.
  - [x] Restore the initial heading of 90 degrees, facing upward.
  - [x] Make `setpos`, `setx`, and `sety` draw when the pen is down.
  - [x] Implement and bind `getwidth`, `setwidth`, `getheight`, and `setheight`.
  - [x] Implement and bind `canvas_size`.
  - [x] Implement and bind `penup`/`pu` and `pendown`/`pd`.
  - [x] Implement and bind `penwidth`/`pw`.
  - [x] Implement and bind named and RGB forms of `pencolor`/`pc`.
  - [x] Implement and bind `home`, `clear`, `reset`, and the compatibility
        no-op `stop`.
  - [x] Implement and bind `forward`/`fd` and `back`/`bk`.
  - [x] Implement and bind `setpos`, `setx`, `sety`, `getx`, and `gety`.
  - [x] Implement and bind `left`/`lt`, `right`/`rt`, and `heading`.
  - [x] Implement and bind `drawme`.
  - [x] Verify `turtle_basic.lua`, `spiral.lua`, `tree.lua`,
        `sierpinski.lua`, and `polygons.lua`.

- [x] Milestone 3: Add state, shapes, timing, and image export
  - [x] Add deep-copy helpers for turtle location, colour, font, and state.
  - [x] Add a dynamically sized saved-state stack to each turtle.
  - [x] Implement and bind `save` and `restore`.
  - [x] Implement the detached TurtleState Lua userdata and its `__gc` and
        `__tostring` behaviour.
  - [x] Implement TurtleState accessors `x`, `y`, `a`, `r`, `g`, `b`,
        `heading`/`hd`, `pd`, and `pw`.
  - [x] Implement and bind `circle` with the current pen colour and width.
  - [x] Port and bind all `arc` forms: radius; radius and extent; radius,
        extent, and steps.
  - [x] Track turtle creation time and bind `elapsed_time_ms`.
  - [x] Implement PNG export from the render texture, including vertical
        orientation correction.
  - [x] Bind PNG export as `export_img` and `snap`.
  - [x] Verify `save_restore.lua` and exported image dimensions and
        orientation.

- [x] Milestone 4: Add text and picture support
  - [x] Define portable font behaviour for bundled fonts, font-file paths, and
        unknown family names.
  - [x] Add a font cache with deterministic ownership and cleanup.
  - [x] Load the Latin-1 glyph range supported by 0.3.0 and document complex
        scripts such as Devanagari as out of scope.
  - [x] Implement and bind `font`.
  - [x] Implement rotated text drawing at the turtle's logical position.
  - [x] Implement and bind `filltext`.
  - [x] Implement `stroketext` with documented compatibility behaviour.
  - [x] Add TurtleImage Lua userdata with deterministic texture cleanup.
  - [x] Implement and bind `loadpic`.
  - [x] Implement `pic` for both an image userdata object and an image path.
  - [x] Verify `draw_mandrill.lua` and `draw_mandrill_move.lua`.
  - [x] Verify `fern.lua` and the Latin rendering in `text-demo.lua`.

- [x] Milestone 5: Restore presentation and animation semantics
  - [x] Define when drawing operations update the visible window.
  - [x] Implement and bind `enable_update` and `disable_update`.
  - [x] Implement and bind `paint` as an immediate presentation and event-poll
        operation.
  - [x] Implement and bind responsive `delay` without freezing the Raylib
        window.
  - [x] Ensure long-running and animated Lua programs continue to process
        window-close events.
  - [x] Define behaviour when the user closes the window while Lua is still
        executing.
  - [x] Verify animation using `draw_mandrill_move.lua`.

- [ ] Milestone 6: Complete compatibility, tests, and release preparation

  Repository promotion and naming:

  - [x] Create an annotated pre-migration tag at the last known-good Raylib
        implementation before removing legacy files.
  - [x] Make `PicoTurtle` and `picoturtle` the only product and executable
        names; remove the experimental `ptrl` and `ptrtl` names.
  - [x] Rename private C types, functions, source files, test targets, and
        diagnostics from `ptrl`/`ptrtl` to `picoturtle` for a coherent
        first-class codebase.
  - [x] Promote the Raylib `src/`, `test/`, `res/`, CMake project, Makefile,
        vcpkg manifests, and triplets from `ptrl/` to the repository root.
  - [x] Move this backlog to the repository root and update every path in it.
  - [x] Set the CMake project version to `0.3.0` and expose the
        `matrix-goose` codename in version/help output.
  - [x] Make root-level `make genbuild`, `make build`, `make test`, and
        `make run` the canonical developer workflow.
  - [x] Ensure the Lua module remains `require("picoturtle")` and the CLI is
        invoked as `picoturtle`.
  - [ ] Define an install layout for the executable, Lua files, bundled fonts,
        and sample resources.
  - [ ] Resolve bundled resources relative to the installed executable or an
        explicit resource override, without depending on the source checkout
        working directory.

  Legacy removal and dependency cleanup:

  - [x] Remove GIF creation from 0.3.0 scope rather than retaining the
        GraphicsMagick dependency; document `picoturtle.makegif` as deprecated.
  - [x] Delete the original Skia/Qt renderer, IDE, editor, REPL, and GUI source
        after the Raylib implementation is promoted.
  - [x] Delete obsolete Qt resources, platform launchers, Skia CMake helpers,
        and Skia/Qt-only license files while preserving reusable images and
        Lua samples.
  - [x] Replace the legacy root CMake, Makefile, and vcpkg manifest rather than
        retaining two competing build systems.
  - [ ] Remove Skia, Qt, GraphicsMagick, `coll`, and `zclk` from all active
        build files and documentation.
  - [x] Replace `coll` and `zclk` usage with minimal direct CLI argument
        parsing.
  - [x] Confirm whether direct GLFW discovery/linking is redundant when
        linking Raylib, and remove it if Raylib already supplies it.
  - [x] Convert the useful `c-color-names` dependency from a Git submodule to
        a pinned vendored source under `third_party/`, retaining its license.
  - [x] Remove `.gitmodules` and stale nested-submodule metadata.
  - [x] Verify that a fresh clone builds without `git submodule` commands.
  - [x] Audit and either update or delete the old Dockerfile, editor settings,
        CI workflows, TODO file, and other root-level legacy configuration.
  - [x] Add a third-party notices section covering Raylib, Lua, Noto Sans, and
        the vendored colour-name data.

  API compatibility audit:

  - [ ] Build a method-and-alias inventory from the original Lua binding and
        compare it with the Raylib binding.
  - [ ] Mark every original public method as supported, deliberately changed,
        or deprecated with a documented replacement.
  - [ ] Confirm argument counts, accepted forms, return values, aliases, and
        Lua error behaviour for every supported method.
  - [ ] Document `picoturtle.makegif` as removed in 0.3.0 and point users to
        PNG snapshots plus an external animation tool.
  - [ ] Document Latin-1 text support and the absence of complex-script
        shaping in 0.3.0.
  - [ ] Run all retained Lua samples and remove or update samples that depend
        on deliberately removed features.
  - [ ] Run maze generation as a large-canvas and text-rendering regression
        test.

  Automated and visual verification:

  - [ ] Add unit tests for locations, colours, headings, movement, pen state,
        state copying, and save/restore.
  - [ ] Add Lua integration tests covering every public method and alias.
  - [ ] Add CLI tests for help, version, invalid arguments, missing files,
        export success, and export failure.
  - [ ] Add resource-lifetime tests for turtles, saved states, fonts, images,
        the canvas, and Lua garbage collection.
  - [ ] Add deterministic reference-image tests for representative lines,
        shapes, state restoration, Latin text, and pictures.
  - [ ] Decide a small, documented pixel-tolerance policy for reference-image
        differences across graphics drivers.
  - [x] Add a clean-build check using `make genbuild` followed by `make build`.
  - [x] Add a root-level test target that runs the entire automated suite.
  - [ ] Repeat the agreed visual checks after the root migration to catch
        resource-path, orientation, font, and animation regressions.

  Documentation, packaging, and release:

  - [ ] Replace the root README with 0.3.0 build, run, export, font,
        editor-independent, and troubleshooting instructions.
  - [ ] Update API documentation and examples for all deliberate compatibility
        differences.
  - [ ] Update the changelog with the Raylib rewrite, removed IDE, removed GIF
        helper, supported text scope, and migration notes.
  - [ ] Ensure the repository license and bundled third-party licenses are
        accurate after legacy removal.
  - [ ] Add CMake install rules and verify macOS runtime resource paths from an
        installed tree outside the source checkout.
  - [ ] Perform a clean macOS Debug build, Release build, install, automated
        test run, sample run, and PNG export.
  - [ ] Verify Windows and Linux builds, or explicitly move them to a named
        post-0.3.0 backlog with the unsupported status documented.
  - [ ] Review the final repository for generated files, obsolete references,
        oversized artifacts, and accidental legacy dependencies.
  - [ ] Push all release commits and the pre-migration safety tag.
  - [ ] Tag and publish `v0.3.0` only after every remaining item above is
        complete or deliberately rescheduled.
