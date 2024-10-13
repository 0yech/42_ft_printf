/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrey <nrey@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 00:19:52 by nrey              #+#    #+#             */
/*   Updated: 2024/10/13 03:07:08 by nrey             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

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

int	ft_printfchar(int c)
{
	write(1, &c, 1);
	return (1);
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
	else if (c == 'd')
		count += ft_printfnbr(va_arg(args, int));
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
	return (count);
}

int	main(void)
{
	int	f;

	f = ft_printf("Hello i am at %d", 42);
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
