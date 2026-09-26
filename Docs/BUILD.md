# Build Notes

Use **MSYS2 UCRT64**, not the plain MSYS shell.

Build:

```bash
./Build/Build_Nyxoryth.bat
```

Clean generated output:

```bash
./Build/Clean.bat
```

Create a portable release:

```bash
./Build/Package_Release.bat
```

The build script runs `Normalize-Timestamps.ps1` before configuration. It only normalizes build-input files whose modification time is ahead of the current system clock.
