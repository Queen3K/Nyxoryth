# Nyxoryth Calculator

**Current release:** 1.1.0

Nyxoryth Calculator is a native Windows calculator designed for local/offline use. It combines a standard calculator with scientific, tip/bill-split, programmer, and computer-utility tools in one application.

## Features

### Standard calculator
- Addition, subtraction, multiplication, and division
- Decimal input, percent, positive/negative toggle, backspace, CE, and C
- Reciprocal, square, and square root
- Chained calculations and operator replacement
- Calculation history
- Keyboard input
- Divide-by-zero and invalid-input handling
- Copy result to clipboard

### Scientific mode
- Degree and radian modes
- sin, cos, tan
- asin, acos, atan
- log and natural log
- factorial
- pi and e
- x^y, 10^x, and e^x
- absolute value

### Scientific+
- sinh, cosh, tanh
- asinh, acosh, atanh
- cube root and nth root
- 2^x and log2
- combinations (nCr) and permutations (nPr)
- modulo
- floor, ceil, and round
- local random-number generation using the C++ standard library

### Statistics
- count, sum, mean, median, and mode
- minimum, maximum, and range
- population variance and standard deviation
- sample variance and standard deviation

### Fractions / GCD / LCM
- fraction addition, subtraction, multiplication, and division
- simplify fractions
- fraction-to-decimal conversion
- improper-to-mixed and mixed-to-improper conversion
- greatest common divisor and least common multiple

### Equation Solver
- linear equations in the form `a*x + b = c`
- quadratic equations in the form `a*x^2 + b*x + c = 0`
- discriminant reporting
- real and complex quadratic roots

### Complex Numbers
- addition, subtraction, multiplication, and division
- magnitude
- conjugate
- argument in radians and degrees

### Tip / Bill Split
- Bill amount
- Custom tip percentage
- 15%, 18%, 20%, and 25% quick presets
- Split by number of people
- Tip amount, total, and per-person result

### Unit Converter
- length, mass, temperature, area, and volume
- speed, pressure, energy, and power
- angle conversion
- decimal and binary data-size conversion (KB/MB/GB/TB and KiB/MiB/GiB/TiB)
- manual-rate conversion for user-supplied exchange/conversion rates without an online API

### Date / Time Tools
- days between two dates
- add or subtract days
- leap-year and day-of-week inspection
- date/time to Unix timestamp (UTC)
- Unix timestamp to UTC date/time
- seconds to hours/minutes/seconds

### Programmer / Computer Tools
- Decimal, hexadecimal, octal, and binary conversion
- AND, OR, XOR, NOT
- Left and right bit shifts
- Byte conversion to KiB, MiB, GiB, and TiB
- IPv4/CIDR network, mask, broadcast, and address count
- Character to Unicode/code-point conversion

### Advanced Bit Tools
- selectable 8/16/32/64-bit word sizes
- signed and unsigned interpretation
- two's-complement bit display
- grouped binary and popcount
- set, clear, and toggle individual bits
- rotate left/right
- endian reversal
- ASCII 0-127 browser
- Unicode scalar-value lookup

### Computer / Storage Math
- transfer-time estimates
- bitrate-to-file-size calculations
- resolution, pixel count, megapixels, aspect ratio, and PPI
- RAID 0/1/5/6/10 usable-capacity estimates and basic fault-tolerance notes

### Navigation and screen layout
- Three-bar menu in the upper-left corner for Standard, Scientific, Scientific+, Statistics, Fractions, Equations, Complex Numbers, Tip, Units, Date/Time, Programmer, Bit Tools, Computer Math, History, and Settings
- Modes open as full calculator screens instead of side-by-side panels
- Scientific mode uses an integrated 5-column keypad containing both scientific and standard calculator keys
- Switching modes no longer automatically widens the window
- Existing normal resize, maximize, restore, and saved window-size support remains available

### Appearance and usability
- Embedded Nyxoryth Windows application icon
- Windows EXE file/product version metadata
- Dark and light themes
- Custom owner-drawn buttons with hover/pressed states
- Explicit light palette for buttons and input/result boxes
- User-selected local PNG, JPG/JPEG, GIF, and BMP backgrounds
- Animated GIF playback without background/control flicker
- Cover and Fit background scaling
- Adjustable readability overlay
- Remembers theme, background, overlay, scale mode, last calculator mode, window position, and Always-on-Top
- Resizable/maximizable window with saved window size
- Optimized animated GIF background rendering and smooth move/resize behavior
- Copy selected history entry
- About screen
- Reset Settings control with confirmation
- `F1` opens About
- `Ctrl+C` copies the current calculator result


## Keyboard shortcuts

Nyxoryth keeps normal text editing behavior inside input boxes while adding screen-level shortcuts:

- `Alt+M` — open the three-bar screen menu
- `F6` — focus/select the primary input for the active tool screen
- `F5` — run the active screen's primary calculation where the screen has a clear primary action
- `Enter` — run the context-appropriate action from single-line tool inputs
- `Ctrl+Enter` — calculate Statistics while preserving ordinary Enter/new-line behavior in the multiline Statistics input
- `F1` — About Nyxoryth
- `Ctrl+C` — copy the calculator result when focus is on the calculator rather than a text-selection field

Examples:
- Equation Solver: Enter in a linear coefficient solves the linear equation; Enter in a quadratic coefficient solves the quadratic equation.
- Date / Time: Enter chooses the action that matches the focused date/time field.
- Computer / Storage Math: Enter runs Transfer, Bitrate, Resolution/PPI, or RAID based on the focused input group.
- Advanced Bit Tools: Enter converts the bit value; Enter in the Unicode field performs the Unicode lookup.

## Custom backgrounds

Nyxoryth does **not** include or distribute custom images, GIFs, JPEGs, PNGs, BMPs, fonts, or other user media.

Background customization lets users select their own local files. Users are responsible for having permission to use any selected file. Nyxoryth stores the selected local file path in its settings and does not upload the file.

## Privacy

Nyxoryth is designed to run locally. The current application does not use telemetry, advertising, cloud calculation, online accounts, or network APIs.

Settings are stored locally under the current Windows user's profile.

## Build requirements

The tested build environment is Windows 11 with **MSYS2 UCRT64**, **MinGW-w64 GCC**, **CMake**, and **Ninja**.

Typical MSYS2 UCRT64 packages:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```

## Build

Open an **MSYS2 UCRT64** terminal in the project folder and run:

```bash
./Build/Build_Nyxoryth.bat
```

The executable is created at:

```text
BuildOutput/Nyxoryth.exe
```

The build script performs a clean build and protects against the future-file-timestamp issue that can cause Ninja to repeatedly regenerate `build.ninja`.

## Portable release package

After a successful build, run:

```bash
./Build/Package_Release.bat
```

This creates:

```text
Release/Nyxoryth_1.1.0_Portable/
Release/Nyxoryth_1.1.0_Portable.zip
Release/Nyxoryth_1.1.0_Portable.zip.sha256
```

The release script verifies required files, generates SHA-256 checksums, and rejects PNG/JPG/JPEG/GIF/BMP user-media files from the portable package.

## Source layout

The active build is intentionally small:

```text
Source/
├── App/
│   └── Main.cpp
├── Resources/
│   ├── Nyxoryth.ico
│   ├── Nyxoryth.rc
│   └── Resource.h
└── UI/
    ├── MainWindow.cpp
    └── MainWindow.h
```

`CMakeLists.txt` uses an explicit source list. Old placeholder/scaffold `.cpp` files are not part of this cleaned project and cannot be silently compiled into the executable.

## Security and copyright contact

For security vulnerabilities, copyright questions, or other project-security concerns, contact:

**Queen3K@proton.me**

For a security report, include the Nyxoryth version, Windows version, reproduction steps, and the impact you observed. Avoid posting sensitive vulnerability details publicly before they can be reviewed.


## Development Transparency

Nyxoryth was developed with assistance from AI tools during the programming process.

AI assistance was used as a development aid for tasks such as brainstorming, debugging assistance, code review, and implementation support. The project owner directed the design, feature decisions, testing, integration, source organization, and final project decisions.

Nyxoryth's active 1.1.0 application source was reviewed for potential third-party code inclusion, copied or paraphrased source material, external licensing requirements, and attribution requirements. No third-party application source code requiring an additional source-code attribution was identified during this review. See `Docs/SOURCE_ATTRIBUTION_AUDIT.md` for the audit scope and limitations.

Nyxoryth uses standard platform APIs and development tools, including Windows APIs and compiler/build tooling. These dependencies are documented separately in `THIRD_PARTY_NOTICES.md`.

## Copyright and Source Concerns

If you believe Nyxoryth contains code, assets, or other material that infringes your rights, please contact the project owner so the concern can be reviewed:

**Queen3K@proton.me**

Please include:

- the specific file or material involved
- a description of the concern
- relevant ownership or licensing information
- contact information for follow-up

Good-faith reports will be reviewed and addressed appropriately.

For security vulnerabilities, please see `SECURITY.md`.

## License

Nyxoryth Calculator is released under the **MIT License**. See [`LICENSE`](LICENSE).

## Third-party software

The Nyxoryth application source does not contain a bundled third-party application framework or copied third-party source module. It uses Windows system APIs and is built with GCC/MinGW-w64. Release builds request static linking of GCC/libstdc++ runtime components where supported; those runtime components are covered by their own licenses and the GCC Runtime Library Exception. See [`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md).

## Project status

Nyxoryth is still under active development. Verify important financial, technical, engineering, or networking calculations before relying on them in high-impact situations.
