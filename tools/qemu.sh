./iso.sh
./mkhdd.sh

qemu-system-i386 -cdrom ../build/os.iso -serial stdio -drive file=../hdd/ext2.img,format=raw
