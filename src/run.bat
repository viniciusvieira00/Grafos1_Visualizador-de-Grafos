@echo off
gcc -g -mwindows main.c -o out

if %errorlevel% neq 0 (
    exit /b %errorlevel%
)

.\out

if %errorlevel% equ 0 (
    echo BATCH:SUCCESS
) else (
    echo BATCH:ERROR
)
