@echo off
setlocal EnableExtensions EnableDelayedExpansion

rem Generate a Visual Studio solution in .\out. Extra arguments are passed to cmake.

set "ROOT=%~dp0"
if "%ROOT:~-1%"=="\" set "ROOT=%ROOT:~0,-1%"

where cmake >nul 2>&1
if errorlevel 1 (
    echo cmake was not found on PATH. 1>&2
    exit /b 1
)

set "GENERATOR="
for /f "delims=" %%L in ('cmake --help 2^>nul ^| findstr /C:"Visual Studio"') do (
    if not defined GENERATOR set "GENERATOR=%%L"
)

if not defined GENERATOR (
    echo cmake did not report a Visual Studio generator. Install Visual Studio with the C++ workload. 1>&2
    exit /b 1
)

set "GENERATOR=!GENERATOR:*=!"
for /f "tokens=* delims= " %%T in ("!GENERATOR!") do set "GENERATOR=%%T"
for /f "tokens=1 delims==" %%G in ("!GENERATOR!") do set "GENERATOR=%%G"

:trim_generator
if "!GENERATOR:~-1!"==" " (
    set "GENERATOR=!GENERATOR:~0,-1!"
    goto trim_generator
)

echo Using !GENERATOR!
cmake -S "%ROOT%" -B "%ROOT%\out" -G "!GENERATOR!" -A x64 %*
exit /b %ERRORLEVEL%
