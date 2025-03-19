@echo off
setlocal

if "%~1"=="" (
    echo No argument provided. Assuming default subdirectory: build
    set "BUILD_DIR=%~dp0build"
) else (
    set "BUILD_DIR=%~dp0%~1"
)

if not exist "%BUILD_DIR%" (
    echo Build directory does not exist: %BUILD_DIR%
    goto :end
)

for /r %%f in (*.cpp *.h) do (
  ::echo Running `clang-tidy -p "%BUILD_DIR%" "%%f"` ...
  clang-tidy -p "%BUILD_DIR%" "%%f"
)

echo All files processed.

:end
endlocal
