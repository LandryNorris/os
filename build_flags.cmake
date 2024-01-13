add_compile_options($<$<COMPILE_LANGUAGE:C>:--target=i386-pc-none-elf>)
add_compile_options($<$<COMPILE_LANGUAGE:C>:-ffreestanding>)
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-felf32>)

add_link_options(--target=i386-pc-none-elf)
add_link_options(-ffreestanding -nostdlib -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-3dnow -Wl,-no-pie)

set(CMAKE_ASM_OBJECT_FORMAT elf32)
