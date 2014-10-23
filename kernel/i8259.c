#include "common.h"

/*
 * 重新映射 IRQ 表
 * 两片级联的 Intel 8259A 芯片
 * 主片端口 0x20 0x21
 * 从片端口 0xA0 0xA1
 */
#define INT_M_PORT1     0x20
#define INT_M_PORT2     0x21
#define INT_S_PORT1     0xA0
#define INT_S_PORT2     0xA1

// 中断向量
#define INT_VECTOR_IRQ0 0x20
#define INT_VECTOR_IRQ8 0x28

void init_8259A()
{
    // Initialize Master, Slave 边沿触发模式
    outb(INT_M_PORT1, 0x11);
    outb(INT_S_PORT1, 0x11);

    // 设置主片 IRQ 从 0x20(32) 号中断开始
    outb(INT_M_PORT2, INT_VECTOR_IRQ0);

    // 设置从片 IRQ 从 0x28(40) 号中断开始
    outb(INT_S_PORT2, INT_VECTOR_IRQ8);

    // 设置主片 IR2 引脚连接从片
    outb(INT_M_PORT2, 0x04);

    // 设置从片输出引脚连接主片 IR2 引脚
    outb(INT_S_PORT2, 0x02);

    // 设置主从片按照 8086 的方式工作
    outb(INT_M_PORT2, 0x01);
    outb(INT_S_PORT2, 0x01);

    // 设置主从片允许中断
    //outb(INT_M_PORT2, 0x0);
    //outb(INT_S_PORT2, 0x0);
    
    // 设置主从片关闭所有中断
    outb(INT_M_PORT2, 0xFC);
    outb(INT_S_PORT2, 0xFF);
}
