#include "memory.h"

u32 load_cr0();
void store_cr0(u32 cr0);

u32 memtest_sub(u32 start, u32 end)
{
    u32 i, *p, old;
    u32 part0 = 0xaa55aa55, part1 = 0x55aa55aa;

    for (i = start; i <= end; i += 0x1000) {    // 每次检查4k
        p = (u32 *)(i + 0xffc); // 只检查末尾4个字节
        old = *p;   // 记录原来的值
        *p = part0; // 试写
        *p ^= 0xffffffff;   // 反转
        if (*p != part1) {  // 检查反转结果
            *p = old;
            break;
        }
        *p ^= 0xffffffff;   // 再次反转
        if (*p != part0) {  // 检查反转值是否恢复
            *p = old;
            break;
        }
        *p = old;
    }
    return i;
}

u32 memtest(u32 start, u32 end)
{
    u32 cr0, i;
    u8 is486;

    // 假设cpu架构是486以上
    is486 = 1;

    if (is486 != 0) {
        cr0 = load_cr0();
        cr0 |= CR0_CACHE_DISABLE;   // 禁止缓存
        store_cr0(cr0);
    }

    i = memtest_sub(start, end);

    if (is486 != 0) {
        cr0 = load_cr0();
        cr0 &= ~CR0_CACHE_DISABLE;   // 允许缓存
        store_cr0(cr0);
    }
    return i;
}
