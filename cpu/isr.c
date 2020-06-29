#include "isr.h"
#include "idt.h"
#include "ports.h"
#include "timer.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../libc/string.h"


isr_t interrupt_handlers[256];

// Can't do this with a loop because we need
// the address of the function names
void isr_install() {
    set_idt_gate(0,  (uint32_t)isr0);
    set_idt_gate(1,  (uint32_t)isr1);
    set_idt_gate(2,  (uint32_t)isr2);
    set_idt_gate(3,  (uint32_t)isr3);
    set_idt_gate(4,  (uint32_t)isr4);
    set_idt_gate(5,  (uint32_t)isr5);
    set_idt_gate(6,  (uint32_t)isr6);
    set_idt_gate(7,  (uint32_t)isr7);
    set_idt_gate(8,  (uint32_t)isr8);
    set_idt_gate(9,  (uint32_t)isr9);
    set_idt_gate(10, (uint32_t)isr10);
    set_idt_gate(11, (uint32_t)isr11);
    set_idt_gate(12, (uint32_t)isr12);
    set_idt_gate(13, (uint32_t)isr13);
    set_idt_gate(14, (uint32_t)isr14);
    set_idt_gate(15, (uint32_t)isr15);
    set_idt_gate(16, (uint32_t)isr16);
    set_idt_gate(17, (uint32_t)isr17);
    set_idt_gate(18, (uint32_t)isr18);
    set_idt_gate(19, (uint32_t)isr19);
    set_idt_gate(20, (uint32_t)isr20);
    set_idt_gate(21, (uint32_t)isr21);
    set_idt_gate(22, (uint32_t)isr22);
    set_idt_gate(23, (uint32_t)isr23);
    set_idt_gate(24, (uint32_t)isr24);
    set_idt_gate(25, (uint32_t)isr25);
    set_idt_gate(26, (uint32_t)isr26);
    set_idt_gate(27, (uint32_t)isr27);
    set_idt_gate(28, (uint32_t)isr28);
    set_idt_gate(29, (uint32_t)isr29);
    set_idt_gate(30, (uint32_t)isr30);
    set_idt_gate(31, (uint32_t)isr31);

    // Remap the PIC
    port_byte_out(PIC1_CTRL, ICW1_INIT | ICW1_ICW4);
    port_byte_out(PIC2_CTRL, ICW1_INIT | ICW1_ICW4);
    port_byte_out(PIC1_DATA, PIC1_OFFSET); // ICW2 - Master vector offset
    port_byte_out(PIC2_DATA, PIC2_OFFSET); // ICW2 - Slave vector offset
    port_byte_out(PIC1_DATA, 0x04); // ICW3 - Tell master PIC there is a slave at IRQ2 (0000 0100)
    port_byte_out(PIC2_DATA, 0x02); // ICW3 - Tell slave PIC its cascade identity (0000 0010)
    port_byte_out(PIC1_DATA, ICW4_8086); // Set 8086/88 mode
    port_byte_out(PIC2_DATA, ICW4_8086);
    port_byte_out(PIC1_DATA, 0x0); // Set masks to 0x0
    port_byte_out(PIC2_DATA, 0x0);

    // Install the IRQs
    set_idt_gate(32, (uint32_t)irq0);
    set_idt_gate(33, (uint32_t)irq1);
    set_idt_gate(34, (uint32_t)irq2);
    set_idt_gate(35, (uint32_t)irq3);
    set_idt_gate(36, (uint32_t)irq4);
    set_idt_gate(37, (uint32_t)irq5);
    set_idt_gate(38, (uint32_t)irq6);
    set_idt_gate(39, (uint32_t)irq7);
    set_idt_gate(40, (uint32_t)irq8);
    set_idt_gate(41, (uint32_t)irq9);
    set_idt_gate(42, (uint32_t)irq10);
    set_idt_gate(43, (uint32_t)irq11);
    set_idt_gate(44, (uint32_t)irq12);
    set_idt_gate(45, (uint32_t)irq13);
    set_idt_gate(46, (uint32_t)irq14);
    set_idt_gate(47, (uint32_t)irq15);

    // load with asm
    set_idt();
}

void irq_install() {
    // enable interrupts
    __asm__ __volatile__("sti");
    // IRQ0: timer
    init_timer(50);
    // IRQ1: keyboard
    init_keyboard();
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

// To print the messages which defines every exception
char *exception_messages[] = {
    "Division by zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(registers_t *r) {
    kprint("Received interrupt: ");
    char s[3];
    int_to_ascii(r->int_no, s);
    kprintln(s);
    kprintln(exception_messages[r->int_no]);
}

void irq_handler(registers_t *r) {
    // After every interrupt we have to send an EOI to the PICs
    // or they will not send another interrupt again.
    // If it's from the slave, both PICs must receive an EOI
    if (r->int_no >= 40) {
        port_byte_out(PIC2_CTRL, 0x20); // Slave
    }
    port_byte_out(PIC1_CTRL, 0x20); // Master

    // Handle the interrupt in a more modular way
    if (interrupt_handlers[r->int_no] != 0) {
        isr_t handler = interrupt_handlers[r->int_no];
        __asm__("nop");
        handler(r);
    }
}