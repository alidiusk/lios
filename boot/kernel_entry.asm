global _start

[bits 32]
_start:
    [extern kmain] ; define calling point, must have same name as kernel.c "main" function
    call kmain ; calls the C function. The linker will know where it is placed in memory.
    jmp $
