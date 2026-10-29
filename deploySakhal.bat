@echo off
setlocal
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0LaunchOfflineMode.ps1" -Map Sakhal %*
if errorlevel 1 (
    pause
    exit /b 1
)
endlocal