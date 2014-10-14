#include "timer.h"
#include "idt.h"
#include "console.h"

void timer_callback(pt_regs *regs)
{
    console_write_color("Timer  ",rc_black, rc_green);
}

void init_timer(u32 frequency)
{
    console_write("Init_timer.\n");
    // 注册时间相关的处理函数
    register_interrupt_handler(IRQ0, timer_callback);

    u32 divisor = 1193180 / frequency;

    // 设置 8253/8254 芯片工作在模式3下
    outb(0x43, 0x36);

    // 拆分低字节和高字节
    u8 low = (u8)(divisor & 0xFF);
    u8 hign = (u8)((divisor >> 8) & 0xFF);
    // 分别写入低字节和高字节
    outb(0x40, low);
    outb(0x40, hign);
}
