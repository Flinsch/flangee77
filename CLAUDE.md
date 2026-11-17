
# CLAUDE.md

Guidance for Claude Code when working in this repository.

---

## Project

flangee77 is a from-scratch C++ game framework (not an engine; see `README.md` and
`docs/personal-intention.md`): APIs are designed for a consumer writing C++ against them
directly, not for drag-and-drop/visual editor tooling. Don't propose visual editors,
scene-graph GUIs, or asset-pipeline tooling as the primary way to use a feature.

Current work is on 2D/text rendering (font rasterization, the `fl7::gui` widget framework),
but the long-term goal is 3D games. Treat a shortcut that's only "good enough for 2D/text"
(e.g., skipping sRGB render targets or physically based blending) as something likely to need
revisiting once 3D/lighting work starts, not as the project's permanent ceiling.

## Code Style

See `docs/code-style-guide.md` for the authoritative code and comment style guide.

## Workflow

- Never build the project or run a built executable on your own initiative, not even for a
  quick visual check. Ask first, every time.
- The user runs `git commit` themselves. Draft the commit message and name the files that
  would be staged; don't stage or commit anything yourself.
- After a design discussion, don't infer blanket permission to implement from a partial or
  ambiguous go-ahead. If one of your own open questions wasn't actually answered, or you'd
  have to make a judgment call to fill a gap, say so and ask instead of deciding it silently.

## Architecture

- A higher-level system that draws (e.g., `fl7::gui`) gets its own abstract, overridable
  renderer interface (`draw_rect`/`draw_text`/etc.), with one concrete default implementation
  that composes the lower-level primitives (`xl7::graphics`, `fl7::fonts::render`) internally.
  Don't wire a lower-level rendering primitive directly into a higher-level consumer.

## Debugging

- For font/rasterizer visual-quality bugs (bitmap/SDF/MSDF), prefer the numeric/ASCII
  raw-buffer dumps in `flangee77-tests/src/FaceLabs/fonts/raster/scratch_inspect.cpp` over
  live-demo screenshots as the primary diagnostic tool. Screenshots are distorted by GPU
  filtering and gamma blending and rarely reveal the root cause on their own.
