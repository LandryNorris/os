; Declare constants for the multiboot header.
MAGIC    equ  0xE85250D6        ; 'magic number' lets bootloader find the header
ARCHITECTURE equ 0              ; Specify i386 architecture
LENGTH equ header_end - header  ; Length of the multiboot header
CHECKSUM equ -(MAGIC + ARCHITECTURE + LENGTH)

VMA_BASE equ 0xC0000000


; The page table is used at both page directory entry 0 (virtually from 0x0
; to 0x3FFFFF) (thus identity mapping the kernel) and page directory entry
; 768 (virtually from 0xC0000000 to 0xC03FFFFF) (thus mapping it in the
; # higher half). The kernel is identity mapped because enabling paging does
; # not change the next instruction, which continues to be physical. The CPU
; # would instead page fault if there was no identity mapping.
VMA_PAGE_DIRECTORY_INDEX equ (VMA_BASE >> 22) ; VMA_BASE / 4MB = 768. This gives us the index in the page directory that refers to our VMA base address.
 
; Declare a multiboot header that marks the program as a kernel. These are magic
; values that are documented in the multiboot standard. The bootloader will
; search for this signature in the first 8 KiB of the kernel file, aligned at a
; 32-bit boundary. The signature is in its own section so the header can be
; forced to be within the first 8 KiB of the kernel file.
section .multiboot
header:
align 8
	dd MAGIC
	dd ARCHITECTURE
	dd LENGTH
	dd CHECKSUM

align 8
    ; framebuffer
    dw 5
    dw 0
    dd 20
    dd 640
    dd 480
    dd 32

align 8
	; end tag
	dw 0
	dw 0
	dd 8
header_end:

; The multiboot standard does not define the value of the stack pointer register
; (esp) and it is up to the kernel to provide a stack. This allocates room for a
; small stack by creating a symbol at the bottom of it, then allocating 16384
; bytes for it, and finally creating a symbol at the top. The stack grows
; downwards on x86. The stack is in its own section so it can be marked nobits,
; which means the kernel file is smaller because it does not contain an
; uninitialized stack. The stack on x86 must be 16-byte aligned according to the
; System V ABI standard and de-facto extensions. The compiler will assume the
; stack is properly aligned and failure to align the stack will result in
; undefined behavior.
section .bootstrap_stack
    align 4096
stack_bottom:
    resb 16384 ; 16 KiB
stack_top:

; Initialize page directory and identity map it.
; Also map us to higher-half.
section .data
	align 4096
	global boot_page_directory
boot_page_directory:
    ; 0x83 = 1 0 0 0 0 1 1, PS=1 (huge page. Points directly. No table). W=1, P=1
    ; Because we specify this as a huge page, we don't need a table. Map lowest
    ; 4MiB to lowest 4MiB (identity mapping)
    dd 0x00000083
    times(VMA_PAGE_DIRECTORY_INDEX - 1) dd 0
    ; Same as above. We place this at VMA_PAGE_DIRECTORY_INDEX to map our
    ; kernel's higher-half memory to the lowest 4MiB.
    dd 0x00000083
    times(1024 - VMA_PAGE_DIRECTORY_INDEX - 1) dd 0

; The linker script specifies _start as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded. It
; doesn't make sense to return from this function as the bootloader is gone.
; Declare _start as a function symbol with the given symbol size.
section .text
    align 16
global _start
_start:
	; The bootloader has loaded us into 32-bit protected mode on a x86
	; machine. Interrupts are disabled. Paging is disabled. The processor
	; state is as defined in the multiboot standard. The kernel has full
	; control of the CPU. The kernel can only make use of hardware features
	; and any code it provides as part of itself. There's no printf
	; function, unless the kernel provides its own <stdio.h> header and a
	; printf implementation. There are no security restrictions, no
	; safeguards, no debugging mechanisms, only what the kernel provides
	; itself. It has absolute and complete power over the
	; machine.

.initialize_pages:
    ; Set cr3 to the address of the boot_page_directory
    mov ecx, (boot_page_directory - VMA_BASE)
    mov cr3, ecx

    ; Enable 4MB large pages
    mov ecx, cr4;
    or ecx, 0x00000010
    mov cr4, ecx

    ; Enable paging and the write-protect bit
    mov ecx, cr0
    or ecx, 0x80010000
    mov cr0, ecx

    ; Jump to higher half with an absolute jump
    lea ecx, [.start_kernel]
    jmp ecx

 .start_kernel:
    ; Unmap the identity mapping as it is now unnecessary.
    mov dword [boot_page_directory + 0], 0

    ; Reload cr3 to force a TLB flush so the changes take effect.
    mov ecx, cr3
    mov cr3, ecx

    ; Set up the stack.
    mov esp, stack_top

    ; Push pointer to multiboot structure
    push ebx
    ; Push magic number
    push eax

    ; Enter the high-level kernel.
    extern kernel_main
    call kernel_main

    ; kernel has exited. We have nothing left to do.
    ; Disable interrupts and enter infinite loop.
    cli
.hang:	hlt
	jmp .hang