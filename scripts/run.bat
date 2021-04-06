@echo off

rem fuck qemu all my homies hate qemu
cd C:\Program Files\qemu

rem monkeypog
qemu-system-x86_64 -no-reboot -d int -cdrom C:\Users\conor\Development\kernel\output\kernel.iso

cd C:\Users\conor\Development\kernel\