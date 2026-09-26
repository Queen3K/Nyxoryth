# Source Cleanup

The v1.0 release-candidate tree contains only active application source/resources:

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

The `.ico` file is Nyxoryth's application icon. It is embedded into the EXE and is not a user background.

Earlier placeholder/scaffold `.cpp` files remain excluded. CMake uses an explicit source/resource list instead of recursive source discovery.
