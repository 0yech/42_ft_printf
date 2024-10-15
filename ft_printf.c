/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrey <nrey@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 00:19:52 by nrey              #+#    #+#             */
/*   Updated: 2024/10/15 06:14:00 by nrey             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include "ft_strdup.c"

int	ft_printfchar(int c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_printfstr(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
	return (i);
}

int	ft_putnbr_base(unsigned long n, char *base)
{
	unsigned long	baselen;
	int				count;

	count = 0;
	baselen = 0;
	while (base[baselen])
		baselen++;
	if (n >= baselen)
	{
		count += ft_putnbr_base(n / baselen, base);
		count += ft_putnbr_base(n % baselen, base);
	}
	else
	{
		ft_printfchar(base[n]);
		count++;
	}
	return (count);
}

static int	ft_printfhex(unsigned int n, char c)
{
	char	*base;
	int		count;

	if (c == 'x')
		base = "0123456789abcdef";
	else if (c == 'X')
		base = "0123456789ABCDEF";
	count += ft_putnbr_base((unsigned long)n, base);
	return (count);
}

static int	ft_printfptr(void *ptr)
{
	int	count;

	count = ft_printfstr("0x");
	count += ft_putnbr_base((unsigned long)ptr, "0123456789abcdef");
	return (count);
}

static size_t	unlen(unsigned int n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		len++;
	while (n)
	{
		len++;
		n /= 10;
	}
	return (len);
}

static size_t   nlen(int n)
{
        size_t  len;

        len = 0;
        if (n <= 0)
                len++;
        while (n)
        {
                len++;
                n /= 10;
        }
        return (len);
}

char	*ft_unsigneditoa(unsigned int n)
{
	char	*s;
	size_t	len;

	len = unlen(n);
	s = malloc(sizeof(char) * (len + 1));
	if (!s)
		return (NULL);
	s[len] = '\0';
	while (n != 0)
	{
		s[--len] = n % 10 + 48;
		n = n / 10;
	}
	return (s);
}

void    appnbr(char *p, int n, int c)
{
        if (n >= 10)
        {
                appnbr(p, n / 10, c - 1);
                n = n % 10;
        }
        p[c] = n + 48;
}

char    *ft_itoa(int n)
{
        char                    *s;
        size_t                  len;
        int                             nb;

        if (n == -2147483648)
                return (ft_strdup("-2147483648"));
        len = nlen(n);
        s = malloc(len + 1);
        if (!s)
                return (NULL);
        s[len] = '\0';
        nb = n;
        if (n < 0)
        {
                s[0] = '-';
                nb = -n;
        }
        appnbr(s, nb, len - 1);
        return (s);
}

int	ft_printfuint(unsigned int n)
{
	char	*p;
	int		count;

	p = ft_unsigneditoa(n);
	count = ft_printfstr(p);
	p = NULL;
	free(p);
	return (count);
}

int	ft_printfnbr(int nb)
{
	char	*p;
	int		count;

	p = ft_itoa(nb);
	count = ft_printfstr(p);
	p = NULL;
	free(p);
	return (count);
}

int	ft_vartype(va_list args, char c)
{
	int count;

	count = 0;
	if (c == 'c')
		count += ft_printfchar(va_arg(args, int));
	else if (c == 's')
		count += ft_printfstr(va_arg(args, char *));
	else if (c == 'd' || c == 'i')
		count += ft_printfnbr(va_arg(args, int));
	else if (c == '%')
		count += ft_printfchar(37);
	else if (c == 'u')
		count += ft_printfuint(va_arg(args, unsigned int));
	else if (c == 'x' || c == 'X')
		count += ft_printfhex(va_arg(args, unsigned int), c);
	else if (c == 'p')
		count += ft_printfptr(va_arg(args, void *));
	return (count);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		count;
	int		i;

	i = 0;
	count = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			count += ft_vartype(args, str[i + 1]);
			i++;
		}
		else
			count += ft_printfchar(str[i]);
		i++;
	}
	va_end(args);
	return (count);
}

int	main(void)
{
	int	f;
	char	*p;

	p = "marche";
	f = ft_printf("Hello i am at %i\n", 42);
	ft_printf("\npercent : %%\nchar : %c\nint : %d\nuint : %u\npointer : %p\nhex : %x\nhexMaj : %X\nstring : %s\n", 'a', 43, 42, p, 892, 892, p);
	return (0);
}
/*

FT_PRINTF MEMO

- Doesn't do buffer management like the real printf.

- will manage cspdiuxX%

--------------------------

  Steps&Tips to achieve

--------------------------

Printing the string like usual. Except if a % is detected.

If so, detect which type (with str[index + 1](%d, %s, ...) and have functions managing each type.

Examples : printstr, printnbr or hex.

These function shall help us count every character that was printed. As
ft_printf is an int function that returns the length of characters it printed succesfully.

Each function should just write() whatever they're getting as arguments.

	ft_printf("Hello my name is %s", "Nelson");
						 ^      ^^       ^
	const char *str -----|    type       |
	va_list args -------------------------

	Our printf function prints the string as usual. Until it detects a percent(%) symbol.
	Percent symbols COULD! call one of our variables, or it could just be printing a real
	percent symbol. That shall be part of the "checking which type" process.

	Luckily, when using printf. The user needs to put their variables in the same order as
	they're called in the *str string. This makes the use of Variadic functions simple.

	! By calling our variadic function with the va_arg(), it will move to the next argument
	automatically for the next use !

	so after checking which type to use, we could do something like this :

	va_start(args, str)					-Sets the start of "next variables count may vary" or "..." after "str" in our prototype.

	ft_printstr(va_arg(args, char *));	-Print the string, and send the first "..." or "Variadic" parameter. Calling va_arg will then make
										it move to the next parameter.

	va_end(args)						-We are done working with the parameters.

The rule stating we shouldn't manage buffer management is great to say the least.

basically, buffer management would have our printf process it's way out in a total different way to improve performance.

The point of our ft_printf is to just simply print characters individually. Which is not the way buffer management works.
*/
