# Third-Party Notices

Nyxoryth's application source is maintained as a small native Win32 codebase and does not bundle a third-party application framework or copied third-party source module.

## Windows system APIs

Nyxoryth uses APIs supplied by Microsoft Windows, including:

- Win32
- GDI+
- Windows common file dialog
- Windows common controls
- Windows-provided Bahnschrift/Bahnschrift SemiBold fonts requested at runtime

Windows is not distributed with this repository. Nyxoryth does not bundle the Bahnschrift font files; it only requests the Windows-installed font at runtime. Microsoft is not claimed as an author of Nyxoryth.

## GCC / libstdc++ runtime components

Nyxoryth is currently built with MinGW-w64 GCC. The Release configuration requests static linking of `libgcc` and `libstdc++` where supported.

Those GCC runtime components are third-party software. GNU libstdc++ is licensed under GPLv3 with the **GCC Runtime Library Exception, version 3.1**. The exception is intended to permit eligible GCC-compiled target code, including non-GPL programs, to be distributed under terms chosen for the independent application code.

Nyxoryth's own application source remains licensed under the MIT License.

If the build or packaging process is changed in the future to redistribute separate GCC/MinGW runtime DLLs or other third-party files, their applicable license files and notices should be reviewed and included as required.

## Build toolchain

The project uses separately installed development tools including:

- MSYS2
- MinGW-w64 / GCC
- CMake
- Ninja

These tools are not authored by the Nyxoryth project and are governed by their respective licenses.

## User-provided assets

Nyxoryth does not bundle user background media. PNG, JPG/JPEG, GIF, BMP, fonts, or other files selected by a user remain the responsibility of that user and are not covered by Nyxoryth's MIT License unless their copyright holder separately licenses them that way.

## Contact

Security and copyright contact: **Queen3K@proton.me**
