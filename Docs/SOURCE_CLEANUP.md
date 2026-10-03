# Source Cleanup

Nyxoryth 1.1.0 uses a deliberately small active native-Windows source tree:

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

`CMakeLists.txt` explicitly names the active application sources/resources. It does not recursively discover source files.

## Public-repository cleanup

Before the 1.1.0 GitHub update, legacy scaffold files from early Nyxoryth development were removed from the public-source tree. Those files were not compiled by the current CMake target and were no longer part of the working implementation.

The cleanup also removed stale configuration/build-note files that described old development states such as the former TOS requirement, old Beta version labels, or Visual Studio build paths.

This keeps the public repository aligned with what is actually built, tested, and released.

## Application icon

`Source/Resources/Nyxoryth.ico` is the Nyxoryth application icon. It was created specifically for this project from geometric shapes and is embedded in the EXE by the Windows resource file. It is not a user background asset.
