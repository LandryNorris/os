./iso.sh

qemu-system-i386 -cdrom ../build/os.iso -serial stdio -hda ../hdd/disk.img
