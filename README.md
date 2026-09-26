# Nyxoryth Calculator

**Current release:** 1.0.0 RC1

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

### Tip / Bill Split
- Bill amount
- Custom tip percentage
- 15%, 18%, 20%, and 25% quick presets
- Split by number of people
- Tip amount, total, and per-person result

### Programmer / Computer Tools
- Decimal, hexadecimal, octal, and binary conversion
- AND, OR, XOR, NOT
- Left and right bit shifts
- Byte conversion to KiB, MiB, GiB, and TiB
- IPv4/CIDR network, mask, broadcast, and address count
- Character to Unicode/code-point conversion

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
- Copy selected history entry
- About screen
- Reset Settings control with confirmation
- `F1` opens About
- `Ctrl+C` copies the current calculator result

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
Release/Nyxoryth_Portable/
Release/Nyxoryth_Portable.zip
Release/Nyxoryth_Portable.zip.sha256
```

The release script verifies required files, generates SHA-256 checksums, and rejects PNG/JPG/JPEG/GIF/BMP user-media files from the portable package.

## Source layout

The active build is intentionally small:

```text
Source/
├── App/
│   └── Main.cpp
└── UI/
    ├── MainWindow.cpp
    └── MainWindow.h
```

`CMakeLists.txt` uses an explicit source list. Old placeholder/scaffold `.cpp` files are not part of this cleaned project and cannot be silently compiled into the executable.

## Security and copyright contact

For security vulnerabilities, copyright questions, or other project-security concerns, contact:

**Queen3K@proton.me**

For a security report, include the Nyxoryth version, Windows version, reproduction steps, and the impact you observed. Avoid posting sensitive vulnerability details publicly before they can be reviewed.

## License

Nyxoryth Calculator is released under the **MIT License**. See [`LICENSE.txt`](LICENSE.txt).

## Third-party software

The Nyxoryth application source does not contain a bundled third-party application framework or copied third-party source module. It uses Windows system APIs and is built with GCC/MinGW-w64. Release builds request static linking of GCC/libstdc++ runtime components where supported; those runtime components are covered by their own licenses and the GCC Runtime Library Exception. See [`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md).

## Project status

Nyxoryth is still under active development. Verify important financial, technical, engineering, or networking calculations before relying on them in high-impact situations.
