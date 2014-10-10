#include "gdt.h"

// 全局描述符表长度
#define GDT_LENGTH  5

// 全局描述符表
gdt_entry_t gdt_entries[GDT_LENGTH];

// GDTR
gdt_ptr_t gdt_ptr;

static void gdt_set_gate(u32 num, u32 base, u32 limit, u8 access, u8 gran);

void init_gdt()
{
    // 全局描述符界限
    gdt_ptr.limit = sizeof(gdt_entry_t) * GDT_LENGTH  - 1;    // 从0开始，所以总长 - 1
    // GDT基地址
    gdt_ptr.base = (u32)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // 指令段 read/exec
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // 数据段 read/write
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // 用户模式指令段 read/exec
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // 用户模式数据段 read/write


    // 加载全局描述符表地址到 GDTR 寄存器
    gdt_flush((u32)&gdt_ptr);
}

// 描述符构造函数
static void gdt_set_gate(u32 num, u32 base, u32 limit, u8 access, u8 gran)
{
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= (gran & 0xF0);
    gdt_entries[num].access = access;
}
