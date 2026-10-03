# Nyxoryth Source / Attribution Audit

Audit date: 2026-10-03  
Release reviewed: 1.1.0

## Scope

This audit covers the public source that is actually compiled into Nyxoryth 1.1.0:

- `Source/App/Main.cpp`
- `Source/UI/MainWindow.cpp`
- `Source/UI/MainWindow.h`
- `Source/Resources/Nyxoryth.rc`
- `Source/Resources/Resource.h`
- `Source/Resources/Nyxoryth.ico`

The active CMake target explicitly references `Main.cpp`, `MainWindow.cpp`, and `Nyxoryth.rc`.

Legacy scaffold source/config files from earlier development iterations were removed before this audited public-repository package was created.

## Source dependency review

The active C++ source uses:

- C++ standard-library headers
- Microsoft Windows / Win32 APIs
- GDI+
- Windows common dialogs
- Windows common controls
- Nyxoryth's own local headers/resources

No third-party application framework, downloaded calculator library, copied source module, network SDK, HTTP library, or calculation API is included in the active application source.

The active source was scanned for embedded third-party copyright/license headers, source URLs, GitHub/Stack Overflow attribution markers, copied-source notices, and similar provenance markers. None were found.

## Implementation review

Nyxoryth-specific systems reviewed include:

- Standard and Scientific calculation paths
- Scientific+
- Statistics
- Fractions / GCD / LCM
- Equation Solver
- Complex Numbers
- Tip / Bill Split
- Unit Converter
- Date / Time Tools
- Programmer / Computer Tools
- Advanced Bit Tools
- Computer / Storage Math
- three-bar screen navigation
- theme/background/settings handling
- animated GIF rendering/performance path
- keyboard handling
- release/settings persistence

The extended 1.1.0 calculation implementation uses standard mathematical formulas and C++ standard-library facilities such as `<cmath>`, `<numeric>`, `<bitset>`, and `<random>`. Mathematical formulas and standard API usage can naturally resemble implementations in unrelated programs; this does not by itself indicate copied source.

No outside implementation was identified as the source of Nyxoryth's project-specific code during this review.

## Network / telemetry review

The active source contains no WinHTTP, WinINet, socket, cURL, HTTP-request, webhook, analytics, or telemetry implementation.

Nyxoryth stores settings locally and lets the user choose local background files.

## Application icon / assets

The Nyxoryth application icon was created specifically for this project from geometric shapes and does not use a downloaded icon, stock artwork, external font asset, or copied vector/image asset.

Nyxoryth does not bundle user background images, GIFs, JPEGs, PNGs, BMPs, or font files.

## Third-party components and credits

Nyxoryth uses the Windows platform and is built with separately installed development tools. These are dependencies/tooling, not authors of Nyxoryth's application source.

Relevant notices are documented in `THIRD_PARTY_NOTICES.md`, including:

- Windows system APIs
- Windows-provided Bahnschrift font requested at runtime
- MSYS2
- MinGW-w64 / GCC
- CMake
- Ninja
- GCC/libstdc++ runtime components and the GCC Runtime Library Exception

No individual third-party code author was identified who needs to be credited as an author of Nyxoryth's application source.

## Copyright / license consistency

The repository MIT license identifies:

`Copyright (c) 2026 Queen3K`

The Windows executable version resource in this audited package uses the same copyright owner.

## Important limitation

This audit is evidence-based, not a guarantee that no generic line or programming pattern resembles code anywhere else in the world. Standard Win32 calls, C++ idioms, formulas, and ordinary control-flow patterns are widely used.

The review found no evidence that Nyxoryth's active application source contains copied or paraphrased third-party application code requiring an additional source-code attribution.

## Files to keep with public releases

- `LICENSE`
- `THIRD_PARTY_NOTICES.md`
- `SECURITY.md`

Security/copyright contact: **Queen3K@proton.me**
