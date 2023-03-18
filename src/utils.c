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

/*
 * ft_fabs: returns the absolute value of a double
 * @num: the number to get the absolute value of
 * @return: the absolute value of the number
 */
double ft_fabs(double num)
{
    return num < 0 ? -num : num;
}

/*
 * ft_sqrt: returns the square root of a double using babylonian method
 * @num: the number to get the square root of
 * @return: the square root of the number
 */
double ft_sqrt(double num)
{
    if (num < 0)
    {
        return -1; // Return -1 for negative input values
    }

    if (num == 0 || num == 1)
    {
        return num; // Return the input value for 0 or 1
    }

    double precision = 1e-10; // Define the precision for the calculation
    double guess = num;
    double previous_guess;

    while (ft_fabs(guess - previous_guess) > precision)
    {
        previous_guess = guess;
        guess = (guess + num / guess) / 2.0;
    }

    return guess;
}

bool ft_isdigit(char c)
{
    return c >= '0' && c <= '9';
}

int ft_isnumber(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!ft_isdigit(str[i]))
        {
            return 0;
        }
    }
    return 1;
}

int ft_atoi(const char *str)
{
    int nb;
    int i;
    int neg;

    neg = 0;
    i = 0;
    nb = 0;
    while (str[i] == '\n' || str[i] == '\t' || str[i] == '\r' || str[i] == '\v' || str[i] == '\f' || str[i] == ' ' || str[i] == '0')
        i++;
    if (str[i] == '-')
        neg = 1;
    if (str[i] == '+' || str[i] == '-')
        i++;
    if (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
        while (str[i] >= '0' && str[i] <= '9')
        {
            nb = (nb * 10) + (str[i] - '0');
            i++;
        }
    if (neg == 1)
        nb *= -1;
    return (nb);
}

int ft_strcmp(const char *s1, const char *s2)
{
    unsigned int i;

    i = 0;
    while ((s1[i] && s2[i]) && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}