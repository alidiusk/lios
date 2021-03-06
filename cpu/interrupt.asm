; Defined in isr.c
[extern isr_handler]
[extern irq_handler]

; Common ISR code
isr_common_stub:
    ; 1. Save the CPU state
    pusha ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
    mov ax, ds ; Lower 16 bits of eax = ds
    push eax ; save the data segment descriptor
    mov ax, 0x10 ; kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp ; registers_t *r

    ; 2. Call C handler
    cld ; C code following the sysV ABI requires DF to be clear on function entry
    call isr_handler

    ; 3. Restore state
    pop eax ; restore esp
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8 ; Cleans up pushed error code and pushed ISR number
    iret ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

; Common IRQ code. Identical to ISR code except for the
; 'call' and the 'pop ebx'
irq_common_stub:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp ; registers_t *r

    ; 2. Call C handler
    cld
    call irq_handler

    ; 3. Restore state
    pop ebx ; restore esp
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    iret

; We don't get information about which interrupt was called
; when the handler is run, so we will need to have a different handler
; for every interrupt.
; Furthermore, some interrupts push an error code onto the stack but others
; don't, so we will push a dummy error code for those which don't, so that
; we have a consistent stack for all of them.

; First make the ISRs global
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

; IRQs
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

; 0: Divide by zero exception
isr0:
    push byte 0
    push byte 0
    jmp isr_common_stub

; 1: Debug exception
isr1:
    push byte 0
    push byte 1
    jmp isr_common_stub

; 2: Non maskable interrupt exception
isr2:
    push byte 0
    push byte 2
    jmp isr_common_stub

; 3: Int 3 exception
isr3:
    push byte 0
    push byte 3
    jmp isr_common_stub

; 4: INTO exception
isr4:
    push byte 0
    push byte 4
    jmp isr_common_stub

; 5: Out of bounds exception
isr5:
    push byte 0
    push byte 5
    jmp isr_common_stub

; 6: Invalid opcode exception
isr6:
    push byte 0
    push byte 6
    jmp isr_common_stub

; 7: Coprocessor not available exception
isr7:
    push byte 0
    push byte 7
    jmp isr_common_stub

; 8: Double fault exception (with error code!)
isr8:
    push byte 0
    push byte 8
    jmp isr_common_stub

; 9: Coprocessor segment overrun exception
isr9:
    push byte 0
    push byte 9
    jmp isr_common_stub

; 10: Bad TSS exception (with error code!)
isr10:
    push byte 10
    jmp isr_common_stub

; 11: Segment not present exception (with error code!)
isr11:
    push byte 11
    jmp isr_common_stub

; 12: Stack fault exception (with error code!)
isr12:
    push byte 12
    jmp isr_common_stub

; 13: General fault protection exception (with error code!)
isr13:
    push byte 13
    jmp isr_common_stub

; 14: Page fault exception (with error code!)
isr14:
    push byte 14
    jmp isr_common_stub

; 15: Reserved exception
isr15:
    push byte 0
    push byte 15
    jmp isr_common_stub

; 16: Floating point exception
isr16:
    push byte 0
    push byte 16
    jmp isr_common_stub

; 17: Alignment check exception
isr17:
    push byte 0
    push byte 17
    jmp isr_common_stub

; 18: Machine check exception
isr18:
    push byte 0
    push byte 18
    jmp isr_common_stub

; 19: exception
isr19:
    push byte 0
    push byte 19
    jmp isr_common_stub

; 20: Reserved exception
isr20:
    push byte 0
    push byte 20
    jmp isr_common_stub

; 21: Reserved exception
isr21:
    push byte 0
    push byte 21
    jmp isr_common_stub

; 22: Reserved exception
isr22:
    push byte 0
    push byte 22
    jmp isr_common_stub

; 23: Reserved exception
isr23:
    push byte 0
    push byte 23
    jmp isr_common_stub

; 24: Reserved exception
isr24:
    push byte 0
    push byte 24
    jmp isr_common_stub

; 25: Reserved exception
isr25:
    push byte 0
    push byte 25
    jmp isr_common_stub

; 26: Reserved exception
isr26:
    push byte 0
    push byte 26
    jmp isr_common_stub

; 27: Reserved exception
isr27:
    push byte 0
    push byte 27
    jmp isr_common_stub

; 28: Reserved exception
isr28:
    push byte 0
    push byte 28
    jmp isr_common_stub

; 29: Reserved exception
isr29:
    push byte 0
    push byte 29
    jmp isr_common_stub

; 30: Reserved exception
isr30:
    push byte 0
    push byte 30
    jmp isr_common_stub

; 31: Reserved exception
isr31:
    push byte 0
    push byte 31
    jmp isr_common_stub

; IRQ handlers
irq0:
    push byte 0
    push byte 32
    jmp irq_common_stub

irq1:
    push byte 1
    push byte 33
    jmp irq_common_stub

irq2:
    push byte 2
    push byte 34
    jmp irq_common_stub

irq3:
    push byte 3
    push byte 35
    jmp irq_common_stub

irq4:
    push byte 4
    push byte 36
    jmp irq_common_stub

irq5:
    push byte 5
    push byte 37
    jmp irq_common_stub

irq6:
    push byte 6
    push byte 38
    jmp irq_common_stub

irq7:
    push byte 7
    push byte 39
    jmp irq_common_stub

irq8:
    push byte 8
    push byte 40
    jmp irq_common_stub

irq9:
    push byte 9
    push byte 41
    jmp irq_common_stub

irq10:
    push byte 10
    push byte 42
    jmp irq_common_stub

irq11:
    push byte 11
    push byte 43
    jmp irq_common_stub

irq12:
    push byte 12
    push byte 44
    jmp irq_common_stub

irq13:
    push byte 13
    push byte 45
    jmp irq_common_stub

irq14:
    push byte 14
    push byte 46
    jmp irq_common_stub

irq15:
    push byte 15
    push byte 47
    jmp irq_common_stub
