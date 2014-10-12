#ifndef INCLUDE_IDT_H
#define INCLUDE_IDT_H

#include "types.h"

//初始化中断描述符表
void init_idt();

//中断描述符
typedef struct idt_gate_t {
    u16 base_low;           // 中断处理函数地址 偏移量 15~0
    u16 select;             // 目标代码段描述符选择子
    u8  always0;            // 置0段
    u8  flags;              // 一些标志
    u16 base_high;          // 中断处理函数地址 偏移量 31 ~ 16
} __attribute__((packed)) idt_gate_t;

//IDTR
typedef struct idt_ptr_t {
    u16 limit;              // 界限
    u32 base;               // 基址
} __attribute__((packed)) idt_ptr_t;

// IDT 加载到 IDTR函数， 由汇编实现
extern void idt_flush(u32);

// 中断发生时需要保护的寄存器类型
typedef struct pt_regs_t {
    u32 ds;         // 用于保护用户的数据段描述符
    u32 edi;        // 从 edi 到 eax 由pusha指令压入
    u32 esi;
    u32 ebp;
    u32 esp;
    u32 ebx;
    u32 edx;
    u32 ecx;
    u32 eax;
    u32 int_no;     // 中断号
    u32 error_code; // 错误代码
    u32 eip;        // 以下由cpu自动压入
    u32 cs;
    u32 eflags;
    u32 useresp;
    u32 ss;
} pt_regs;

// 定义中断处理函数指针
// interrupt_handler_t 是一个指向函数的指针，该函数具有一个参数pt_regs *类型的参数，返回值是void
typedef void (*interrupt_handler_t)(pt_regs *);

// 注册一个中断处理函数，即将一个中断处理函数放入数组中
void register_interrupt_handler(u8 n, interrupt_handler_t handler);

// 调用处理函数, 处理ISR
void isr_handler(pt_regs *regs);

// 声明中断处理函数, 汇编实现
// ISR中断服务程序:(Interrupt service routine)
// 0 ~ 19 属于CPU的异常
void isr0();
void isr1();
void isr2();
void isr3();
void isr4();
void isr5();
void isr6();
void isr7();
void isr8();
void isr9();
void isr10();
void isr11();
void isr12();
void isr13();
void isr14();
void isr15();
void isr16();
void isr17();
void isr18();
void isr19();

// 20 ~ 31 属于Intel 保留，未使用
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

// 32 ~ 255 属于用户自定义中断
void isr255();

#endif //INCLUDE_IDT_H
