# Nyxoryth Extended Calculation Screen Checklist

This checklist covers the extended calculation screens included in Nyxoryth 1.1.0.

## Screen navigation
- Open the three-bar menu and confirm these screens appear:
  - Standard
  - Scientific
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
  - History
  - Settings / Appearance
- Switch through every screen repeatedly and confirm the window does not auto-widen.
- Manually resize/maximize/restore Nyxoryth and confirm the selected screen remains usable.
- With an animated GIF background, drag and resize the window and confirm the existing performance/flicker fixes remain intact.

## Scientific+
- X=0 -> sinh -> `0`
- X=-8 -> cbrt -> `-2`
- X=27, Y=3 -> nth root -> `3`
- X=5, Y=2 -> nCr -> `10`
- X=5, Y=2 -> nPr -> `20`
- X=10, Y=3 -> mod -> `1`
- X=2.9 -> floor -> `2`
- X=2.1 -> ceil -> `3`
- X=2.5 -> round -> `3`
- Random with X=10, Y=20 should produce a finite value between 10 and 20.

## Statistics
Use `1, 2, 2, 3`:
- Count: `4`
- Sum: `8`
- Mean: `2`
- Median: `2`
- Mode: `2` with frequency `2`
- Min/Max: `1 / 3`
- Range: `2`
- Population variance: `0.5`
- Population std dev: approximately `0.707106781186548`
- Sample variance: approximately `0.666666666666667`

## Fractions / GCD / LCM
- A=1/2, B=1/3 -> Add -> `5/6`
- A=6/8 -> Simplify -> `3/4`
- A=1/2 -> Decimal -> `0.5`
- A=7/3 -> Mixed -> `2 1/3`
- Mixed `1 1/2` -> Improper -> `3/2`
- A numerator=12, B numerator=18 -> GCD `6`, LCM `36`
- Division by a zero fraction should show an error instead of crashing.

## Equation Solver
- Linear a=2, b=5, c=15 -> x=`5`
- Quadratic a=1, b=-3, c=2 -> roots `2` and `1`
- Quadratic a=1, b=0, c=1 -> complex roots `0 ± 1i`

## Complex Numbers
Use z1=`3 + 4i`, z2=`1 - 2i`:
- Add -> `4 + 2i`
- Subtract -> `2 + 6i`
- Multiply -> `11 - 2i`
- Magnitude z1 -> `5`
- Conjugate z1 -> `3 - 4i`
- Argument should be approximately `0.927295218001612` radians / `53.130102354156` degrees.

## Unit Converter
- 1 mile -> kilometers -> `1.609344`
- 100 °C -> °F -> `212`
- 1 GiB -> GB -> `1.073741824`
- Manual Rate: Base -> Target, value=10, rate=1.5 -> `15`
- Manual Rate: Target -> Base, value=15, rate=1.5 -> `10`

## Date / Time Tools
- Date A `2026-10-03`, Date B `2027-01-01` -> `90` days
- `1970-01-01 00:00:00` -> Unix timestamp `0`
- Unix timestamp `0` -> `1970-01-01 00:00:00 UTC`
- Duration `3661` -> `1 h 1 m 1 s`
- Inspect `2024-02-29` -> leap year Yes and Thursday
- Add 1 day to `2024-02-28` -> `2024-02-29`

## Programmer / Computer Tools regression
- Existing integer conversion still works.
- Existing AND/OR/XOR/NOT and shift tools still work.
- Existing storage, CIDR, and character-code tools still work.

## Advanced Bit Tools
- Value `255`, 8-bit -> unsigned `255`, signed `-1`, popcount `8`
- Value `0x81`, 8-bit -> ROL 1 -> `0x3`
- Value `0x12345678`, 32-bit -> Endian reverse -> `0x78563412`
- Set/Clear/Toggle reject bit indices outside the selected word size.
- Unicode `65` or `0x41` -> `U+0041` / `A`
- ASCII list scrolls from 0 through 127.

## Computer / Storage Math
- Transfer: 10 GB at 100 Mbps -> `800` seconds (13 m 20 s)
- Bitrate: 3600 seconds at 8 Mbps -> `3600 MB`, approximately `3.35276126861572 GiB`
- Display: 1920×1080 at 24 inches -> 2.0736 MP, 16:9, approximately 91.7878 PPI
- RAID 5: 4 drives × 4 TB -> `12 TB` usable estimate
- RAID 10 should reject odd drive counts.

## Themes / backgrounds regression
- Light mode still has no black control rectangles/corners.
- Dark mode still restores correctly.
- PNG/JPG/BMP backgrounds still work.
- Animated GIF backgrounds still animate without child-control flicker.
