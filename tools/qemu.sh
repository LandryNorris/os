./iso.sh
./mkhdd.sh

qemu-system-i386 -cdrom ../build/os.iso -serial stdio -drive file=../hdd/disk.img,format=raw
