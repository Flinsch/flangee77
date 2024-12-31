@echo off

set BUILD_DIR=%~dp0%1

for /r %%f in (*.cpp *.h) do (
  rem echo Running `clang-tidy -p "%BUILD_DIR%" "%%f"` ...
  clang-tidy -p "%BUILD_DIR%" "%%f"
)

echo All files processed.
