#!/bin/bash

rm -rf isodir
rm -rf output

# make temp directories
mkdir -p isodir/boot/grub
mkdir output

# copy files
cp tools/boot/grub/grub.cfg isodir/boot/grub/grub.cfg
cp cmake-build-debug/kernel isodir/boot/kernel.bin

# make iso
grub-mkrescue -o output/kernel.iso isodir

# cleanup
rm -rf isodir

