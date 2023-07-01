Operating System
================

This is a hobby operating system to
build experience with low-level development.

The current build tooling uses clang for
C and nasm for assembly files.

Building
--------

This project uses make. To build the
kernel, run `make build/os.bin`, or
just `make`. This will create a file
os.bin in the build folder

To build a bootable iso file, run
`./iso.sh` in the tools folder. This 
will create a bootable iso in the 
build folder.

To run via QEMU, run `/qemu.sh` in the
tools folder. You will see a QEMU 
menu pop up asking you to select an
entry. Select 'os' and the Operating
System will start.
