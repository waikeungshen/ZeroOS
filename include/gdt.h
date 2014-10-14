#ifndef INCLUDE_GDT_H_
#define INCLUDE_GDT_H_

#include "types.h"

typedef struct gdt_entry_t {
    u16 limit_low;      // 段界限   15 ~ 0
    u16 base_low;       // 段基地址 15 ~ 0
    u8  base_middle;    // 段基地址 23 ~ 16
    u8  access;         // 段存在位、描述符特权级、描述符类型、描述符子类别
    u8  granularity;    // 其他标志、段界限 19 ~ 16
    u8  base_high;      // 段基地址 31 ~ 24
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_ptr_t {
    u16 limit;          // 全局描述符表限长
    u32 base;           // 全局描述符表 32 位基地址
} __attribute__((packed)) gdt_ptr_t;

// 初始化全局描述符表(GDT)
void init_gdt();

// GDT 加载到 GDTR 的函数, 由汇编实现
extern void gdt_flush(u32);

#endif //INCLUDE_GDT_H_
