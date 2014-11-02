#include "string.h"

inline void memset(void *dest, u8 val, u32 len)
{
    u8 *dst = (u8 *)dest;
    for (; len != 0; len--)
    {
        *dst++ = val;
    }
}

inline void bzero(void *dest, u32 len)
{
    memset(dest, 0, len);
}
