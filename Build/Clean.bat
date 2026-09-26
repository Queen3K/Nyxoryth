@echo off
setlocal
for %%I in ("%~dp0..") do set "ROOT=%%~fI"

if exist "%ROOT%\BuildOutput" rmdir /s /q "%ROOT%\BuildOutput"
if exist "%ROOT%\Release" rmdir /s /q "%ROOT%\Release"

echo Nyxoryth generated build/release output removed.
pause
