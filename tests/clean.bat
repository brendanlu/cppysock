@REM Clean all tests exes in this directory
@echo off
cd /d "%~dp0"

for %%i in (*.exe) do (
    del "%%i"
    echo Deleted "%%i"
)

echo Cleanup complete.