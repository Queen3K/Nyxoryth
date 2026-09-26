# Changelog

## 1.0.0 RC1

### Contact / notices
- Added copyright/security contact to Windows EXE version details: `Queen3K@proton.me`
- Added the same contact to About Nyxoryth
- Added `SECURITY.md`
- Added `SECURITY.md` to the verified portable release
- Expanded GCC/libstdc++ runtime notice

### Release hardening
- Added an original Nyxoryth Windows application icon
- Added Windows EXE file/product version metadata
- Added Reset Settings with confirmation
- Added recovery for saved window coordinates that are no longer on a connected monitor
- Added graceful recovery when a saved background file was moved or deleted
- Fixed About-dialog line breaks
- Added portable-release verification
- Added SHA-256 checksum generation for the EXE and portable ZIP

### Confirmed UI fixes carried forward
- Light mode explicitly paints light button/control backgrounds
- Rounded buttons no longer expose black corners
- Animated GIF backgrounds use double-buffered painting
- Animated GIF updates do not repaint/flicker child controls

### Existing application
- Standard calculator
- Scientific calculator
- Tip / bill split
- Programmer / computer tools
- History and clipboard actions
- Dark / Light themes
- Local PNG/JPG/JPEG/GIF/BMP backgrounds
- Cover/Fit scaling and readability overlay
- Settings persistence
- Always-on-Top
- Last-mode persistence
- Window-position persistence
- About screen
- Clean explicit CMake source list
