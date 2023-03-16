#include "ft_ping.h"

void *ft_memset(void *s, int c, size_t n)
{
    unsigned char *p = s;
    while (n--)
    {
        *p++ = (unsigned char)c;
    }
    return s;
}

void ft_bzero(void *s, size_t n)
{
    ft_memset(s, 0, n);
}

void *ft_memcpy(void *dest, const void *src, size_t n)
{
    char *dest_ptr = dest;
    const char *src_ptr = src;

    for (size_t i = 0; i < n; i++)
    {
        dest_ptr[i] = src_ptr[i];
    }

    return dest;
}