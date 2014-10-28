#ifndef INCLUDE_MEMORY_H
#define INCLUDE_MEMORY_H

#include "types.h"

#define EFLAGS_AC_BIT        0x00040000
#define CR0_CACHE_DISABLE    0X60000000

u32 memtest (u32 start, u32 end);

#endif //INCLUDE_MEMORY_H
