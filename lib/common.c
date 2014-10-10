#include "common.h"

// 端口写一个字节
inline void outb(u16 port, u8 value)
{
    asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

// 端口读一个字节
inline u8 inb(u16 port)
{
    u8 ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// 端口读一个字
inline u16 inw(u16 port)
{
    u16 ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}
