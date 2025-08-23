/* linker.ld */
ENTRY(start)

SECTIONS {
    . = 0x100000; /* Kernel load address */
    .text : {
        *(.multiboot)
        *(.text)
    }
    .rodata : {
        *(.rodata)
    }
    .data : {
        *(.data)
    }
    .bss : {
        *(.bss)
    }
    /DISCARD/ : {
        *(.note.GNU-stack)
    }
}