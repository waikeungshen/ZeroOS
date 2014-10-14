#include "console.h"

typedef __builtin_va_list va_list;

#define va_start(ap, last)         (__builtin_va_start(ap, last))
#define va_arg(ap, type)           (__builtin_va_arg(ap, type))
#define va_end(ap) 

static int vsprintf(char *buff, const char *format, va_list args);

void printk(const char *format, ...)
{
    static char buff[1024];
    va_list args;

    va_start(args, format);
    i = vsprintf(buff, format, args);
    va_end(args);

    buff[i] = '\0';

    console_write(buff);
}

static char *number(char *str, int num)
{
    const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
}

int vsprintf(char *buff, const char *format, va_list args)
{
    int len;
    int i;
    char *str;
    char *s;
    int *ip;

    int flags;  // flags to number();

    int field_width;    // width of output field
    int precision;

    int qualifier;        /* 'h', 'l', or 'L' for integer fields */


    for (str=buff; *format; ++format)   // str为最终存放字符串的位置,buff始终指向最终字符串的起始位置
    {
        if(*format != '%')          // 如果不是%则直接复制
        {
            *str++ = *format;
            continue;
        }

        ++format;   //this also skips first %

        switch(*format)
        {
            case 'c':
                *str++ = (char)va_arg(args, int);
                break;
            case 's':
                s = va_arg(args, char*);
                len = strlen(s);
                for (i = 0; i < len; i++)
                    *str++ = *s++;
                break;
            case 'd':
                str = number(str, va_arg(args, long))
                break;
            default:
                if (*format != '%')
                    *str++ = '%';
                if (*format)
                    *str++ = *format;
                else
                    --format;
                break;
        }
    }
    *str = '\0';

    return str - buff;  // 返回为字符串的长度

}
