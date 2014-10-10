//#include "types.h"
#include "common.h"
#include "console.h"

// VGA 的显示缓冲的起点是 0xB8000
static u16 *video_memory = (u16 *)0xB8000;

// 屏幕光标的坐标
static u8 cursor_x = 0;
static u8 cursor_y = 0;

static void move_cursor()
{
    u16 cursorLocation = cursor_y * 80 + cursor_x;

    outb(0x3D4, 14);
    outb(0x3D5, cursorLocation >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cursorLocation);
}

// 清屏，用黑底白字的空格覆盖整个屏幕，并将光标移动到 (0,0) 位置
void console_clear()
{
    // 空格 ASCII 0x20
    // 黑色 0 白色 15
    u8 attribute = (0 << 4) | (15  & 0x0F);
    u16 blank = 0x20 | (attribute << 8);   // 黑底白字 空格 0x0F20

    int i;
    for (i = 0; i < 80 * 25; i++)
    {
        video_memory[i] = blank;
    }

    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

// 屏幕滚动
static void scroll()
{
    u8 attribute = (0 << 4) | (15  & 0x0F);
    u16 blank = 0x20 | (attribute << 8);   // 黑底白字 空格 0x0F20

    int i;
    if (cursor_y >= 25)
    {
        // 将所有行数据复制到上一行
        for (i = 0*80; i < 24 * 80; i++)
        {
            video_memory[i] = video_memory[i+80];
        }
        // 最后一行填充空格
        for(i = 24 * 80; i < 25 * 80; i++)
        {
            video_memory[i] = blank;
        }
        cursor_y = 24;
    }
}

// 显示一个字符，带颜色
void console_putc_color(char c, real_color_t back, real_color_t fore)
{
    u8 back_color = (u8)back;
    u8 fore_color = (u8)fore;

    // 属性
    u8 attribute_byte = (back_color << 4) | (fore_color & 0x0F);
    u16 attribute = attribute_byte << 8;

    // 0x08 退格
    // 0x09 tab
    if (c == 0x08 && cursor_x)
    {
        cursor_x--;
    }
    else if (c == 0x09)
    {
        cursor_x = (cursor_x + 8) & ~(8-1);
    }
    else if (c == '\r')
    {
        cursor_x = 0;
    }
    else if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else if (c >= ' ')
    {
        video_memory[cursor_y * 80 + cursor_x] = c | attribute;
    }

    // 每80个字符一行，满就换行
    if (cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y++;
    }

    scroll();       // 如果有需要，滚屏
    move_cursor();  // 移动光标

}

void console_write(char *str)
{
    while(*str)
    {
        console_putc_color(*str++, rc_black, rc_white);
    }
}

void console_write_color(char *str, real_color_t back, real_color_t fore)
{
    while(*str)
    {
        console_putc_color(*str++, back, fore);
    }
}
