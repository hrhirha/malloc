#include "malloc.h"

void	ft_putn(size_t n, int base)
{
	const char	digits[] = "0123456789abcdef";
	char		s[16];
	int			i;

	i = 0;
	while (1)
	{
		s[i++] = digits[n % base];
		n /= base;
		if (n == 0)
			break ;
	}
	if (base == 16)
		write(1, "0x", 2);
	while(i--)
		write(1, &s[i], 1);
}

void	ft_puts(char *s)
{
	while (*s != 0)
		write(1, s++, 1);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*str1;
	const unsigned char	*str2;

	str1 = dest;
	str2 = src;
	if (dest == src)
		return (dest);
	i = 0;
	while (i < n)
	{
		str1[i] = str2[i];
		i++;
	}
	return (dest);
}
