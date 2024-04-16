@echo off
gcc -g -mwindows main.c -o graph_vizualizer

if %errorlevel% neq 0 (
    exit /b %errorlevel%
)

.\graph_vizualizer

if %errorlevel% equ 0 (
    echo BATCH:SUCCESS
) else (
    echo BATCH:ERROR
)
