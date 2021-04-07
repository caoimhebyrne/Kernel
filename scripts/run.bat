@echo off

rem fuck qemu all my homies hate qemu
pushd C:\Program Files\qemu

rem monkeypog
qemu-system-x86_64 -no-reboot -d int -cdrom %~dp0\..\output\kernel.iso

popd