# Nyxoryth Release Process

## Build

```bash
./Build/Build_Nyxoryth.bat
```

## Verify

Use `TESTS/1.1.0_Release_Checklist.md` for release verification and `TESTS/Extended_Calculation_Checklist.md` for detailed calculator-function checks.

## Create portable release

```bash
./Build/Package_Release.bat
```

The release script:
1. performs a clean Release build,
2. copies only the executable and required project notices,
3. generates an EXE SHA-256 checksum,
4. verifies the portable folder,
5. rejects PNG/JPG/JPEG/GIF/BMP files from the portable folder,
6. creates the portable ZIP,
7. generates a SHA-256 checksum for the ZIP.

The application icon is embedded in `Nyxoryth.exe`; it is not copied as a standalone image into the portable release.
