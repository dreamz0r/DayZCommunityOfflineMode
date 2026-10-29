@echo off
setlocal
set "COM_PROJECT_ROOT=%~dp0..\.."
if not exist "%COM_PROJECT_ROOT%\LaunchOfflineMode.ps1" set "COM_PROJECT_ROOT=P:\DayZCommunityOfflineMode"
if not exist "%COM_PROJECT_ROOT%\LaunchOfflineMode.ps1" (
    echo Cannot find LaunchOfflineMode.ps1. Start the deploy launcher from the project folder.
    pause
    exit /b 1
)
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%COM_PROJECT_ROOT%\LaunchOfflineMode.ps1" -Map Enoch %*
if errorlevel 1 (
    pause
    exit /b 1
)
endlocal