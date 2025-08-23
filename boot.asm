; boot.asm
section .multiboot
align 4
dd 0x1BADB002       ; Magic number
dd 0x00             ; Flags
dd -(0x1BADB002 + 0x00) ; Checksum

section .text
global start
extern kmain        ; Kernel entry point

start:
    mov esp, stack_top ; Set up stack
    call kmain         ; Call kernel
    cli                ; Disable interrupts
.halt:
    hlt                ; Halt CPU
    jmp .halt

section .bss
align 16
stack_bottom:
resb 16384          ; 16 KB stack
stack_top:

section .note.GNU-stack ; Prevent executable stack