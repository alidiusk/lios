#ifndef ISR_H
#define ISR_H

#include "types.h"

// ISRS reserved for CPU exceptions
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
// IRQ definitions
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

// Define common definitions
#define PIC1 0x20 // IO base address for master PIC
#define PIC2 0xA0 // IO base address for slave PIC
#define PIC1_CTRL PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_CTRL PIC2
#define PIC2_DATA (PIC2 + 1)

#define PIC_EOI 0x20 // End-of-interrupt command code

// reinitialize the PIC controllers, giving them specified vector offsets
// rather than 8h and 70h, as configured by default
#define ICW1_ICW4	0x01		// ICW4 (not) needed
#define ICW1_SINGLE	0x02		// Single (cascade) mode
#define ICW1_INTERVAL4	0x04	// Call address interval 4 (8)
#define ICW1_LEVEL	0x08		// Level triggered (edge) mode
#define ICW1_INIT	0x10		// Initialization - required!

#define ICW4_8086	0x01		// 8086/88 (MCS-80/85) mode
#define ICW4_AUTO	0x02		// Auto (normal) EOI
#define ICW4_BUF_SLAVE	0x08	// Buffered mode/slave
#define ICW4_BUF_MASTER	0x0C	// Buffered mode/master
#define ICW4_SFNM	0x10		// Special fully nested (not)

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

// Struct which aggregates many registers
typedef struct {
    uint32_t ds; // data segment
    uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax; // Pushed by 'pusha'
    uint32_t int_no, err_code; // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, esp, ss; // Pushed by processor automatically
} registers_t;

void isr_install();
void irq_install();

typedef void (*isr_t)(registers_t*);
void register_interrupt_handler(uint8_t n, isr_t handler);

#endif
