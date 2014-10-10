#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include "types.h"

// 端口写一个字节
void outb(u16 port, u8 value);

// 端口读一个字节
u8 inb(u16 port);

// 端口读一个字
u16 inw(u16 port);

#endif // INCLUDE_COMMON_H_
