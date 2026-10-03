# Changelog

## 1.1.0 — 2026-10-03

### Input validation and keyboard polish
- Statistics now reports the exact invalid value/token instead of treating malformed input as an empty data set.
- Statistics protects against extreme display-range results and limits one calculation to 100,000 values.
- Fraction inputs now report which numerator/denominator is invalid and explicitly identify zero denominators.
- Equation Solver checks linear/quadratic output ranges and protects quadratic discriminant/root calculations from overflow.
- Complex Numbers detects undefined `arg(0 + 0i)` and reports numeric overflow cleanly.
- Unit Converter rejects temperatures below absolute zero and requires a positive manual conversion rate.
- Advanced Bit Tools reports when a positive input is masked down to the selected word size.
- Computer / Storage Math now reports invalid unit/RAID selections and bitrate/capacity overflow instead of silently returning.
- Added screen-aware keyboard handling: `Alt+M`, `F5`, `F6`, Enter, and Statistics `Ctrl+Enter`.

### Public-source audit and cleanup
- Re-audited the active 1.1.0 source for third-party attribution/provenance markers.
- Removed inactive legacy scaffold source/config files from the GitHub-ready tree.
- Removed stale Beta/TOS/Visual-Studio-era notes that no longer describe the application.
- Aligned Windows EXE copyright metadata with the MIT `LICENSE` owner (`Queen3K`).
- Documented the Windows-provided Bahnschrift font as a runtime platform dependency; no font file is bundled.

### Release preparation
- Promoted the tested development build from `1.1.0 Preview` to `1.1.0`.
- Updated EXE product metadata and `VERSION.txt`.
- Added versioned portable package names and release notes to the verified release package.

### Expanded calculator screens
- Added Scientific+ with hyperbolic/inverse-hyperbolic functions, cube/nth roots, 2^x, log2, nCr, nPr, modulo, floor/ceil/round, and local random-number generation.
- Added Statistics with mean, median, mode, min/max/range, sum/count, and population/sample variance and standard deviation.
- Added Fractions / GCD / LCM with arithmetic, simplification, decimal conversion, mixed/improper conversion, GCD, and LCM.
- Added Equation Solver for linear and quadratic equations, including complex quadratic roots.
- Added Complex Numbers with arithmetic, magnitude, conjugate, and argument.
- Added Unit Converter for length, mass, temperature, area, volume, speed, pressure, energy, power, angles, data sizes, and user-supplied manual rates.
- Added Date / Time Tools for date differences, day offsets, leap-year/day-of-week checks, Unix timestamps, and duration conversion.
- Added Advanced Bit Tools with 8/16/32/64-bit views, signed/unsigned interpretation, two's complement, popcount, individual bit editing, rotate operations, endian reversal, ASCII browsing, and Unicode lookup.
- Added Computer / Storage Math for transfer time, bitrate/file size, display resolution/aspect/PPI, and RAID capacity estimates.
- All new tools are local/offline and use the existing three-bar screen navigation rather than side-by-side panels.

### GIF/background performance
- Reuses a persistent off-screen background buffer instead of allocating a full-window bitmap for every paint.
- Re-renders the cached background only when the GIF frame, theme, background settings, image, or client size changes.
- GIF timer paints are asynchronous/coalesced instead of forcing an immediate full-window redraw.
- GIF animation pauses while the window is being dragged or interactively resized, then resumes automatically.
- GIF animation also pauses while the window is minimized.
- Animated backgrounds use a lighter high-quality scaling path than static backgrounds.

### Resizable window
- Added normal resizable borders and a maximize button.
- Nyxoryth can be enlarged, maximized, restored, and manually resized back to the minimum size required by the currently visible panels.
- Screen modes use the main calculator area instead of side-by-side panels, so switching modes does not automatically widen the window.
- Window width and height are now saved alongside window position.
- Maximized windows save their normal restored size instead of overwriting the saved size with the maximized rectangle.

## 1.0.0 RC1

### Contact / notices
- Added copyright/security contact to Windows EXE version details: `Queen3K@proton.me`
- Added the same contact to About Nyxoryth
- Added `SECURITY.md`
- Added `SECURITY.md` to the verified portable release
- Expanded GCC/libstdc++ runtime notice

### Release hardening
- Added an original Nyxoryth Windows application icon
- Added Windows EXE file/product version metadata
- Added Reset Settings with confirmation
- Added recovery for saved window coordinates that are no longer on a connected monitor
- Added graceful recovery when a saved background file was moved or deleted
- Fixed About-dialog line breaks
- Added portable-release verification
- Added SHA-256 checksum generation for the EXE and portable ZIP

### Confirmed UI fixes carried forward
- Light mode explicitly paints light button/control backgrounds
- Rounded buttons no longer expose black corners
- Animated GIF backgrounds use double-buffered painting
- Animated GIF updates do not repaint/flicker child controls

### Existing application
- Standard calculator
- Scientific calculator
- Tip / bill split
- Programmer / computer tools
- History and clipboard actions
- Dark / Light themes
- Local PNG/JPG/JPEG/GIF/BMP backgrounds
- Cover/Fit scaling and readability overlay
- Settings persistence
- Always-on-Top
- Last-mode persistence
- Window-position persistence
- About screen
- Clean explicit CMake source list
