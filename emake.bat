@echo off

mkdir output
clang .\src\main.cpp -c -O3 -o .\output\main.obj -D_TUEFI -fno-builtin -fno-exceptions -fno-stack-protector -fshort-wchar -mno-red-zone -target x86_64-pc-win32-coff
lld-link .\output\main.obj -subsystem:efi_application -nodefaultlib -dll -out:.\output\main.dll
if errorlevel 1 (
    exit /b 1
)

mkdir output\fs\
mkdir output\fs\EFI
mkdir output\fs\EFI\BOOT
copy /y .\output\main.dll .\output\fs\EFI\BOOT\BOOTX64.EFI
if errorlevel 1 (
    exit /b 1
)

if "%1"=="run" (
"C:\Program Files\qemu\qemu-system-x86_64" -bios "C:\Program Files\qemu\OVMF.fd" -hda fat:rw:output\fs
)