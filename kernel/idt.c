#include "idt.h"
#include "console.h"
#include "string.h"

// 中断描述符表
idt_gate_t idt_gates[256];

// IDTR
idt_ptr_t idt_ptr;

// 中断处理函数的指针数组，用来保存中断处理函数的指针地址
interrupt_handler_t interrupt_handlers[256];

// 设置中断描述符
static void idt_set_gate(u8 num, u32 base, u16 select, u8 flags);

void isr6_callback(pt_regs *reg);
void isr13_callback(pt_regs *reg);
// 初始化中断描述符表
void init_idt()
{
    init_8259A();

    bzero((u8 *)&interrupt_handlers, sizeof(interrupt_handler_t) * 256);

    idt_ptr.limit = sizeof(idt_gate_t) * 256 - 1;
    idt_ptr.base = (u32)&idt_gates;
    
    bzero((u8 *)&idt_gates, sizeof(idt_gate_t) * 256);

    // 0 ~ 32 用于CPU的中断处理
    idt_set_gate(0,  (u32)isr0,  0x08, 0x8E);
    idt_set_gate(1,  (u32)isr1,  0x08, 0x8E);
    idt_set_gate(2,  (u32)isr2,  0x08, 0x8E);
    idt_set_gate(3,  (u32)isr3,  0x08, 0x8E);
    idt_set_gate(4,  (u32)isr4,  0x08, 0x8E);
    idt_set_gate(5,  (u32)isr5,  0x08, 0x8E);
    idt_set_gate(6,  (u32)isr6,  0x08, 0x8E);
    idt_set_gate(7,  (u32)isr7,  0x08, 0x8E);
    idt_set_gate(8,  (u32)isr8,  0x08, 0x8E);
    idt_set_gate(9,  (u32)isr9,  0x08, 0x8E);
    idt_set_gate(10, (u32)isr10, 0x08, 0x8E);
    idt_set_gate(11, (u32)isr11, 0x08, 0x8E);
    idt_set_gate(12, (u32)isr12, 0x08, 0x8E);
    idt_set_gate(13, (u32)isr13, 0x08, 0x8E);
    idt_set_gate(14, (u32)isr14, 0x08, 0x8E);
    idt_set_gate(15, (u32)isr15, 0x08, 0x8E);
    idt_set_gate(16, (u32)isr16, 0x08, 0x8E);
    idt_set_gate(17, (u32)isr17, 0x08, 0x8E);
    idt_set_gate(18, (u32)isr18, 0x08, 0x8E);
    idt_set_gate(19, (u32)isr19, 0x08, 0x8E);
    idt_set_gate(20, (u32)isr20, 0x08, 0x8E);
    idt_set_gate(21, (u32)isr21, 0x08, 0x8E);
    idt_set_gate(22, (u32)isr22, 0x08, 0x8E);
    idt_set_gate(23, (u32)isr23, 0x08, 0x8E);
    idt_set_gate(24, (u32)isr24, 0x08, 0x8E);
    idt_set_gate(25, (u32)isr25, 0x08, 0x8E);
    idt_set_gate(26, (u32)isr26, 0x08, 0x8E);
    idt_set_gate(27, (u32)isr27, 0x08, 0x8E);
    idt_set_gate(28, (u32)isr28, 0x08, 0x8E);
    idt_set_gate(29, (u32)isr29, 0x08, 0x8E);
    idt_set_gate(30, (u32)isr30, 0x08, 0x8E);
    idt_set_gate(31, (u32)isr31, 0x08, 0x8E);

    idt_set_gate(32, (u32)irq32, 0x08, 0x8E);
    idt_set_gate(33, (u32)irq33, 0x08, 0x8E);
    idt_set_gate(34, (u32)irq34, 0x08, 0x8E);
    idt_set_gate(35, (u32)irq35, 0x08, 0x8E);
    idt_set_gate(36, (u32)irq36, 0x08, 0x8E);
    idt_set_gate(37, (u32)irq37, 0x08, 0x8E);
    idt_set_gate(38, (u32)irq38, 0x08, 0x8E);
    idt_set_gate(39, (u32)irq39, 0x08, 0x8E);
    idt_set_gate(40, (u32)irq40, 0x08, 0x8E);
    idt_set_gate(41, (u32)irq41, 0x08, 0x8E);
    idt_set_gate(42, (u32)irq42, 0x08, 0x8E);
    idt_set_gate(43, (u32)irq43, 0x08, 0x8E);
    idt_set_gate(44, (u32)irq44, 0x08, 0x8E);
    idt_set_gate(45, (u32)irq45, 0x08, 0x8E);
    idt_set_gate(46, (u32)irq46, 0x08, 0x8E);
    idt_set_gate(47, (u32)irq47, 0x08, 0x8E);

    // 255 将来实现系统调用
    idt_set_gate(255, (u32)isr255, 0x08, 0x8E);

    // 更新设置中断描述符表
    idt_flush((u32)&idt_ptr);

    //register_interrupt_handler(6, isr6_callback);
    register_interrupt_handler(13, isr13_callback);
}

// 设置中断描述符
static void idt_set_gate(u8 num, u32 base, u16 select, u8 flags)
{
    idt_gates[num].base_low = base & 0xFFFF;
    idt_gates[num].base_high = (base >> 16) & 0xFFFF;

    idt_gates[num].select = select;
    idt_gates[num].always0 = 0;

    idt_gates[num].flags = flags;   // 这里暂时设置成特权级为0

}

// 将中断处理函数指针放入数组
void register_interrupt_handler(u8 n, interrupt_handler_t handler)
{
    interrupt_handlers[n] = handler;
}

// 调用中断处理函数, 处理ISR
void isr_handler(pt_regs *regs)
{
    if (interrupt_handlers[regs->int_no])   // 如果对应的处理函数存在
    {
        interrupt_handlers[regs->int_no](regs); // 由相应的处理函数处理
    }
    else
    {
        console_write_color("Int number : ", rc_black, rc_green);
        console_write_dec(regs->int_no, rc_black, rc_green);
        console_write_color("No Function to deal with this interrupt!\n", rc_black, rc_red);
    }
}

// 调用中断处理函数, 处理IRQ
void irq_handler(pt_regs *regs)
{
    if (regs->int_no >= 40)
    {
        outb(0xA0, 0x20);   // 发送重设信号给从片
    }
    outb(0x20, 0x20);       // 发送重设信号给主片

    if (interrupt_handlers[regs->int_no])   // 如果对应的处理函数存在
    {
        interrupt_handlers[regs->int_no](regs); // 由相应的处理函数处理
    }
    else
    {
        console_write_color("Int number : ", rc_black, rc_red);
        console_write_dec(regs->int_no, rc_black, rc_red);
        console_write_color("No Function to deal with this interrupt request!\n", rc_black, rc_green);
    }
}

// 处理6号中断
void isr6_callback(pt_regs *reg)
{
    console_write("Int number 6\n");
}
void isr13_callback(pt_regs *reg)
{
    //console_write("Int number 13\n");
}
