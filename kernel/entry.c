#include "console.h"

int kernel_entry()
{
    console_clear();

    console_write_color("Hello, OS kernel!\n", rc_black, rc_green);
    
    return 0;
}
