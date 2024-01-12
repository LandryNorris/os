cd ..

if grub-file --is-x86-multiboot2 $1; then
  echo multiboot confirmed
else
  echo the file is not multiboot
  exit 1
fi

mkdir -p ./build/iso/boot
mkdir ./build/iso/boot/grub

cp $1 ./build/iso/boot/os.bin
cp ./grub/grub.cfg ./build/iso/boot/grub/grub.cfg

grub-mkrescue -o build/os.iso ./build/iso
