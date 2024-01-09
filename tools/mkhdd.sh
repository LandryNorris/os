rm -r ../hdd

mkdir ../hdd

mke2fs -t ext2 -L 'data' -d ../disk/ -o linux -r 1 ../hdd/ext2.img 4G
