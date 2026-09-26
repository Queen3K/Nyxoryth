# Nyxoryth 1.0.0 RC1 Verification Checklist

## Build / metadata
- Run `./Build/Build_Nyxoryth.bat`
- Confirm `BuildOutput/Nyxoryth.exe`
- Confirm the Nyxoryth icon appears in File Explorer
- EXE Properties -> Details:
  - Product name: Nyxoryth Calculator
  - File version: 1.0.0.0
  - Product version: 1.0.0 RC1

## Standard
- `7 + 7 =` -> `14`
- `9 * 9 =` -> `81`
- `2.5 + 1.25 =` -> `3.75`
- `200 + 10 % =` -> `220`
- `5 / 0 =` -> divide-by-zero error
- Backspace, CE, C, +/- and decimal still work

## Scientific
- DEG: `30` -> sin -> `0.5`
- `100` -> log -> `2`
- `5` -> n! -> `120`
- `2 x^y 8 =` -> `256`

## Tip
- Bill `80`, tip `20`, people `2`
- Tip -> `$16.00`
- Total -> `$96.00`
- Per person -> `$48.00`

## Programmer / computer tools
- `42` -> HEX `2A`, OCT `52`, BIN `101010`
- `12 AND 10` -> `8`
- `1073741824` bytes -> `1 GiB`
- `192.168.1.10 /24` -> network `192.168.1.0`

## Themes / backgrounds
- Light mode has no black button backgrounds/corners
- Result/expression/input boxes become white in Light mode
- Dark mode restores the dark palette
- PNG/JPG/JPEG/BMP backgrounds work
- Animated GIF works without background or control flicker
- Cover/Fit and Overlay work

## Stale background recovery
- Select a background, close Nyxoryth, move/rename that file, reopen Nyxoryth
- Nyxoryth starts normally and clears the stale saved background path

## Reset Settings
- Change theme/background/overlay/Always-on-Top
- Click Reset Settings -> No: nothing changes
- Click Reset Settings -> Yes:
  - Dark theme
  - Background cleared, user file NOT deleted
  - Cover mode
  - Overlay 45%
  - Always-on-Top Off
  - Standard mode
  - Compact window

## Clipboard / About
- Copy Result works
- Ctrl+C copies result
- Copy Selected history works
- F1 opens About
- About displays `1.0.0 RC1` with normal line breaks

## Portable release
- Run `./Build/Package_Release.bat`
- Verification reports success
- Confirm:
  - `Release/Nyxoryth_Portable/Nyxoryth.exe`
  - `Release/Nyxoryth_Portable/SHA256SUMS.txt`
  - `Release/Nyxoryth_Portable.zip`
  - `Release/Nyxoryth_Portable.zip.sha256`
- Portable folder contains no PNG/JPG/JPEG/GIF/BMP user background files
