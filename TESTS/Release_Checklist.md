# Nyxoryth 0.9.0 Preview Verification Checklist

## Build
- Run `./Build/Build_Nyxoryth.bat`
- Confirm `BuildOutput/Nyxoryth.exe` exists
- Confirm Ninja does not enter a repeated `Re-running CMake` loop

## Standard
- `7 + 7 =` -> `14`
- `9 * 9 =` -> `81`
- `2.5 + 1.25 =` -> `3.75`
- `200 + 10 % =` -> `220`
- `5 + 5 + 2 =` -> `12`
- `5 / 0 =` -> divide-by-zero error

## Scientific
- DEG: `30` then `sin` -> `0.5`
- `100` then `log` -> `2`
- `5` then `n!` -> `120`
- `2 x^y 8 =` -> `256`

## Tip
- Bill `80`
- Tip `20`
- People `2`
- Tip -> `$16.00`
- Total -> `$96.00`
- Per person -> `$48.00`

## Programmer
- `42` -> HEX `2A`, OCT `52`, BIN `101010`
- `12 AND 10` -> `8`
- `1073741824` bytes -> `1 GiB`
- `192.168.1.10 /24` -> network `192.168.1.0`

## Light mode
- Switch to Light
- Number buttons become white/light instead of remaining dark
- Operator buttons use a light lavender accent
- Result and expression boxes are white
- Tip/Programmer input fields are white
- History list is white
- Settings buttons are light
- Hover and pressed states remain visible
- Switch back to Dark and confirm the dark palette restores

## Backgrounds
- PNG loads
- JPG/JPEG loads
- BMP loads
- Animated GIF plays without flashing the background
- Animated GIF does not make buttons/boxes flicker
- Cover and Fit work
- Overlay cycles through the supported values
- Clear Background works

## Persistence
- Theme persists after restart
- Background path persists
- Cover/Fit persists
- Overlay persists
- Last calculator mode persists
- Always-on-Top persists
- Window position persists

## Clipboard / About
- Copy Result copies current result
- Ctrl+C copies current result
- History -> select an entry -> Copy Selected copies it
- F1 opens About
- Settings -> About Nyxoryth opens About

## Release package
- Run `./Build/Package_Release.bat`
- Confirm `Release/Nyxoryth_Portable/Nyxoryth.exe`
- Confirm `Release/Nyxoryth_Portable.zip`
- Confirm no user background image/GIF is included
