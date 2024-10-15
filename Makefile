# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nrey <nrey@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/15 14:23:38 by nrey              #+#    #+#              #
#    Updated: 2024/10/15 15:13:23 by nrey             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libftprintf.a
CC=cc
INCLUDE=include
LIBFTDIR=libft
SRCDIR=src
CFLAGS=-Wall -Werror -Wextra -g
AR=ar
ARFLAGS=rcs
RM=rm -f
CFILES=$(SRCDIR)/ft_printf.c \
	   $(SRCDIR)/ft_printf_put.c \
	   $(SRCDIR)/ft_printf_xp.c

OFILES=$(CFILES:.c=.o)

all: $(NAME)

$(NAME): $(OFILES)
	@echo "Compiling Libft"
	@$(MAKE) -C $(LIBFTDIR)
	@echo "Making $(NAME)"
	@cp $(LIBFTDIR)/libft.a $(NAME)
	@$(AR) $(ARFLAGS) $(NAME) $(OFILES)
	@echo "$(NAME) Created succesfully"

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(LIBFTDIR) -c $< -o $@

clean:
	$(RM) $(OFILES)
	@$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	$(RM) $(NAME)
	@$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re
