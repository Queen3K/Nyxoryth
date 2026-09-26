@echo off
setlocal

for %%I in ("%~dp0..") do set "ROOT=%%~fI"
cd /d "%ROOT%"

set "NO_PAUSE="
if /I "%~1"=="--no-pause" set "NO_PAUSE=1"

echo =====================================
echo Nyxoryth Calculator Build
echo MinGW-w64 + Ninja
echo =====================================
echo.

if exist "%ROOT%\Build\Normalize-Timestamps.ps1" (
    echo Checking project timestamps...
    powershell -NoProfile -ExecutionPolicy Bypass -File "%ROOT%\Build\Normalize-Timestamps.ps1" "%ROOT%"
    if errorlevel 1 (
        echo Warning: timestamp normalization returned an error.
        echo Continuing with a clean build...
    )
    echo.
)

if exist "%ROOT%\BuildOutput" (
    echo Removing old build output...
    rmdir /s /q "%ROOT%\BuildOutput"
)

echo Configuring CMake...
cmake -G Ninja -S "%ROOT%" -B "%ROOT%\BuildOutput" -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
    echo.
    echo CMake configuration failed.
    if not defined NO_PAUSE pause
    exit /b 1
)

echo.
echo Building Nyxoryth...
cmake --build "%ROOT%\BuildOutput" --parallel
if errorlevel 1 (
    echo.
    echo Build failed.
    if not defined NO_PAUSE pause
    exit /b 1
)

echo.
if not exist "%ROOT%\BuildOutput\Nyxoryth.exe" (
    echo Build completed, but Nyxoryth.exe was not found.
    if not defined NO_PAUSE pause
    exit /b 1
)

echo =====================================
echo Nyxoryth build complete
echo =====================================
echo EXE:
echo %ROOT%\BuildOutput\Nyxoryth.exe
echo.

if not defined NO_PAUSE pause
exit /b 0
