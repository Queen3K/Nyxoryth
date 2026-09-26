@echo off
setlocal

for %%I in ("%~dp0..") do set "ROOT=%%~fI"
cd /d "%ROOT%"

echo =====================================
echo Nyxoryth v1.0 RC1 Portable Release
echo =====================================
echo.

call "%ROOT%\Build\Build_Nyxoryth.bat" --no-pause
if errorlevel 1 (
    echo.
    echo Release packaging stopped because the build failed.
    pause
    exit /b 1
)

set "RELEASE_ROOT=%ROOT%\Release"
set "PORTABLE=%RELEASE_ROOT%\Nyxoryth_Portable"
set "ZIP=%RELEASE_ROOT%\Nyxoryth_Portable.zip"
set "ZIP_HASH=%RELEASE_ROOT%\Nyxoryth_Portable.zip.sha256"

if exist "%PORTABLE%" rmdir /s /q "%PORTABLE%"
if exist "%ZIP%" del /q "%ZIP%"
if exist "%ZIP_HASH%" del /q "%ZIP_HASH%"

mkdir "%PORTABLE%"

copy /y "%ROOT%\BuildOutput\Nyxoryth.exe" "%PORTABLE%\Nyxoryth.exe" >nul
copy /y "%ROOT%\README.md" "%PORTABLE%\README.md" >nul
copy /y "%ROOT%\LICENSE" "%PORTABLE%\LICENSE" >nul
copy /y "%ROOT%\THIRD_PARTY_NOTICES.md" "%PORTABLE%\THIRD_PARTY_NOTICES.md" >nul
copy /y "%ROOT%\SECURITY.md" "%PORTABLE%\SECURITY.md" >nul
copy /y "%ROOT%\VERSION.txt" "%PORTABLE%\VERSION.txt" >nul

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$h=(Get-FileHash '%PORTABLE%\Nyxoryth.exe' -Algorithm SHA256).Hash; Set-Content -Path '%PORTABLE%\SHA256SUMS.txt' -Value ($h + '  Nyxoryth.exe') -Encoding ASCII"
if errorlevel 1 (
    echo Failed to create executable checksum.
    pause
    exit /b 1
)

powershell -NoProfile -ExecutionPolicy Bypass -File ^
  "%ROOT%\Build\Verify_Release.ps1" "%PORTABLE%"
if errorlevel 1 (
    echo.
    echo Release verification failed.
    pause
    exit /b 1
)

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "Compress-Archive -Path '%PORTABLE%\*' -DestinationPath '%ZIP%' -Force"
if errorlevel 1 (
    echo.
    echo ZIP creation failed.
    pause
    exit /b 1
)

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$h=(Get-FileHash '%ZIP%' -Algorithm SHA256).Hash; Set-Content -Path '%ZIP_HASH%' -Value ($h + '  Nyxoryth_Portable.zip') -Encoding ASCII"
if errorlevel 1 (
    echo.
    echo ZIP checksum creation failed.
    pause
    exit /b 1
)

echo.
echo =====================================
echo Release package verified and complete
echo =====================================
echo Portable folder:
echo %PORTABLE%
echo.
echo ZIP:
echo %ZIP%
echo.
echo ZIP SHA256:
echo %ZIP_HASH%
echo.
pause
