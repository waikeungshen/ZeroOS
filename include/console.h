#ifndef INCLUDE_CONSOLE_H_
#define INCLUDE_CONSOLE_H_

#include "types.h"

typedef enum real_color {
    rc_black = 0,
    rc_blue  = 1,
    rc_green = 2,
    rc_cyan  = 3,
    rc_red   = 4,
    rc_magenta = 5,
    rc_brown = 6,
    rc_white = 15

} real_color_t;

// 清屏操作
void console_clear();

// 输出一个字符带颜色
void console_putc_color(char c, real_color_t back, real_color_t fore);

// 输出一个字符串，默认黑底白字
void console_write(char *str);
void console_write_color(char *str, real_color_t back, real_color_t fore);

//输出一个十进制整数
void console_write_dec(u32 n, real_color_t back, real_color_t fore);

#endif
