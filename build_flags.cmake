
function(setupEmbeddedFlags target)
    target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C>:--target=i386-pc-none-elf>)
    target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C>:-ffreestanding>)
    target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:ASM>:-felf32>)

    target_link_options(${target} PRIVATE --target=i386-pc-none-elf)
    target_link_options(${target} PRIVATE -ffreestanding -nostdlib -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-3dnow -Wl,-no-pie)
endfunction()

set(CMAKE_ASM_OBJECT_FORMAT elf32)
