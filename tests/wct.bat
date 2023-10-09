@REM Compile c++ windows tests in this directory
@echo off
if "%~1"=="" (
  echo Usage: compile.bat <source_file> [<output_executable>]
  exit /b 1
)

set source_file=%1
if "%~2"=="" (
  set output_executable=%~n1
) else (
  set output_executable=%2
)

echo g++ -D_WIN32 %source_file% -o %output_executable% -lws2_32
g++ -D_WIN32 %source_file% -o %output_executable% -lws2_32

@REM Check if the compilation was successful
if %errorlevel% equ 0 (
  echo Compilation successful.
  echo Running %output_executable%
  %output_executable%
) else (
  echo Compilation failed.
)