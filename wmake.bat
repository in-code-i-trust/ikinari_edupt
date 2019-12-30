@echo off
setlocal enabledelayedexpansion

mkdir output
clang .\src\main.cpp -c -O3 -o .\output\main.obj -D_TWIN32 -fno-builtin -fno-exceptions -fno-stack-protector -fshort-wchar -mno-red-zone
lld-link .\output\main.obj kernel32.Lib -subsystem:windows -nodefaultlib -out:.\output\main.exe
if errorlevel 1 (
    exit /b 1
)

if "%1"=="run" (
echo !time!
start /wait .\output\main.exe
echo !time!
)