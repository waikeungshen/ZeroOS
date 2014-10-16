#include "console.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"

int kernel_entry()
{
    init_gdt();     // 初始化GDT
    init_idt();     // 初始化IDT

    console_clear();

    console_write_color("Hello, OS kernel!\n", rc_black, rc_green);
    
    asm volatile ("int $0x3");   

    asm volatile ("sti"); // 开启中断

    //init_timer(200);

    asm volatile ("int $0x21");   
    asm volatile ("int $0x3");   
    return 0;
}
