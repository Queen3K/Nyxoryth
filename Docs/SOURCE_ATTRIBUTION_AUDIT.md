# Nyxoryth Source / Attribution Audit

Date: 2026-09-26

## Scope reviewed

The cleaned v1.0 RC1 project contains only two active C++ implementation files plus the Windows resource file:

- `Source/App/Main.cpp`
- `Source/UI/MainWindow.cpp`
- `Source/UI/MainWindow.h`
- `Source/Resources/Nyxoryth.rc`

## Source includes

The application source includes only:

- Windows / GDI+ / common-dialog / common-control headers
- C++ standard-library headers
- Nyxoryth's own local headers/resources

No third-party application framework header or third-party source module is included in the active source tree.

## Attribution scan

The active source tree was checked for embedded copyright headers, third-party license headers, source URLs, Stack Overflow attribution, GitHub-source attribution, or copied-library notices. None were found in Nyxoryth's application source.

A limited public-web uniqueness spot check was also performed against distinctive Nyxoryth-specific identifiers and phrases. No matching external source for those distinctive project-specific strings was identified.

This is not a mathematical proof that every generic Win32/C++ expression is globally unique. Standard API usage and commonplace programming patterns can naturally resemble other software.

## Third-party components that still matter

The project uses Microsoft Windows system APIs. Those APIs are platform dependencies/interfaces, not Nyxoryth source authors.

The Release build requests static linking of GCC/libstdc++ runtime components where supported. Those runtime components are third-party code and are covered by their own licenses, including the GCC Runtime Library Exception. `THIRD_PARTY_NOTICES.md` records this.

## Current credit conclusion

No individual third-party code author was identified who needs to be credited as an author of Nyxoryth's application source.

Keep these project files when distributing releases:

- `LICENSE`
- `THIRD_PARTY_NOTICES.md`
- `SECURITY.md`

Copyright/security contact: Queen3K@proton.me
