#include "console.h"
#include "gdt.h"

int kernel_entry()
{
    init_gdt();     // 初始化GDT

    console_clear();

    console_write_color("Hello, OS kernel!\n", rc_black, rc_green);
    
    return 0;
}
