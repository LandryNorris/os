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

Debugging
---------

IDEs like CLion provide helpful debugging 
tools. If configured correctly, QEMU can
act as a GDB remote debugger. To configure
this, make sure to run `make debug` to
create a build with debug symbols. Then
make sure to add the `-s -S` flags, which
tell QEMU to wait for a gdb connection.

### Manual GDB connection

to connect manually, run `gdb` in the console.
Then run `file kernel/build/os.bin` so gdb
knows what it's debugging. If the gdb console
says 'Reading symbols from kernel/build/os.bin...',
the debug symbols are loaded. If it gives an
error, try to `make clean` and `make debug`
and see if it sees the symbols now. When you
have loaded the symbols, run `target remote 
localhost:1234` so gdb knows how to connect to
the remote debugger. The default port is 1234.
If gdb prints 'Remote debugging using localhost:1234'
and a hexadecimal address (the symbol name may be
'??'), then you are good to continue. To
tell QEMU you are ready to start the kernel,
run `continue` in the console. From here, you
can use gdb as normal.

### CLion

Download the 'Embedded development support'
plugin from settings and create a 'Embedded GDB
server' run configuration. Set the configuration
options as below:

```
Target: debugiso
Executable Binary: <path to os.bin>
Debugger: Bundled GDB (multiarch)
Download Executable: If updated
'target remote' args: localhost:1234
GDB server: qemu-system-i386
GDB server args: -s -S -cdrom <path to iso>
```

Click the 'Debug' button in the Ide to
start QEMU. Breakpoints and other debugging
tools within CLion will work.

If breakpoints are disabled when the kernel
is running, check that the target is set to 
debugiso. If it is, and breakpoints are still
disabled, try a `make clean` first to remove
any non-optimized builds.

