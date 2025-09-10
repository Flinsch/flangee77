@echo off
setlocal ENABLEDELAYEDEXPANSION

:: Check if clang-tidy is available.
where clang-tidy >nul 2>&1
if errorlevel 1 (
    echo ERROR: clang-tidy not found in PATH.
    echo.
    echo Please ensure Clang tools are installed and clang-tidy is accessible from the command line.
    echo.
    echo If you already have clang-tidy installed via a development environment, consider adding it to your PATH.
    echo Possible locations where it may already exist:
    echo  - C:\Program Files\JetBrains\CLion 2024.3\bin\clang\win\x64\bin
    echo  - C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\bin
    echo.
    echo Alternatively, you can download Clang tools from: https://clang.llvm.org/
    goto :end
)

:: Handle argument for build directory.
if "%~1"=="" (
    echo No argument provided. Assuming default subdirectory: build
    set "BUILD_DIR=%~dp0build"
) else (
    set "BUILD_DIR=%~dp0%~1"
)

:: Normalize path.
set "BUILD_DIR=%BUILD_DIR:"=%"

if not exist "%BUILD_DIR%" (
    echo ERROR: Build directory does not exist: "%BUILD_DIR%"
    goto :end
)

:: Define the output file name, e.g., using a timestamp for uniqueness.
for /f "delims=" %%a in ('powershell -Command "Get-Date -Format 'yyyy-MM-dd_HH-mm-ss'"') do set "OUTPUT_FILE=clang-tidy-output-%%a.log"

:: Run clang-tidy on all relevant files.
for /r %%f in (*.cpp *.h) do (
    set "FILE=%%~nxf"

    echo !FILE! | findstr /i "CMakeCXXCompilerId.cpp" >nul
    if not errorlevel 1 (
        :: Skip CMake's generated files.
        echo Skipping CMake file: %%f
    ) else (
        ::echo Running `clang-tidy -p "%BUILD_DIR%" "%%f"` ...
        clang-tidy -p "%BUILD_DIR%" "%%f" 2>&1 | powershell -Command "$input | Tee-Object -FilePath '%OUTPUT_FILE%' -Append"
    )
)

echo.
echo All files processed.

:end
endlocal
