[bits 16]

switch_to_pm:
    cli ; disable interrupts
    lgdt [gdt_descriptor] ; load gdt descriptor
    mov eax, cr0
    or eax, 0x01 ; set 32bit mode in cr0 by setting the first bit
    mov cr0, eax
    jmp CODE_SEG:init_pm ; 4. far jump by using a different segment
                         ; to flush the CPU pipeline

[bits 32]
init_pm: ; now using 32bit instructions
    mov ax, DATA_SEG ; 5. update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000 ; update the stack register right at the top of free space
    mov esp, ebp

    call BEGIN_PM ; call some well known label
