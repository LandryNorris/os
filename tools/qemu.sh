cd ..
mkdir cmake-build
cd cmake-build
cmake ..
cmake --build . --target iso

qemu-system-i386 -cdrom ../build/os.iso -serial stdio -drive file=../hdd/ext2.img,format=raw
