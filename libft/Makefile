# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/22 14:53:05 by vihane            #+#    #+#              #
#    Updated: 2025/03/28 19:25:39 by vgalmich         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	libft.a

INCLUDE	=	includes/libft.h includes/ft_print.h includes/get_next_line.h

SRCS        :=      sources/ft_str/ft_strchr.c \
                          sources/ft_str/ft_strdup.c \
                          sources/ft_str/ft_striteri.c \
                          sources/ft_str/ft_strjoin.c \
                          sources/ft_str/ft_strlcat.c \
                          sources/ft_str/ft_strlcpy.c \
                          sources/ft_str/ft_strlen.c \
                          sources/ft_str/ft_strmapi.c \
                          sources/ft_str/ft_strncmp.c \
                          sources/ft_str/ft_strcmp.c \
                          sources/ft_str/ft_strnstr.c \
                          sources/ft_str/ft_strchr.c \
						  sources/ft_str/ft_strrchr.c \
                          sources/ft_str/ft_strtrim.c \
                          sources/ft_str/ft_substr.c \
                          sources/ft_str/ft_strndup.c \
                          sources/ft_str/ft_split.c \
                          sources/ft_to/ft_tolower.c \
                          sources/ft_to/ft_toupper.c \
                          sources/ft_to/ft_atoi.c \
						  sources/ft_to/ft_atof.c \
                          sources/ft_to/ft_itoa.c \
                          sources/ft_put/ft_putchar_fd.c \
                          sources/ft_put/ft_putendl_fd.c \
                          sources/ft_put/ft_putnbr_fd.c \
                          sources/ft_put/ft_putstr_fd.c \
                          sources/ft_gnl/get_next_line_utils.c \
                          sources/ft_gnl/get_next_line.c \
                          sources/ft_is/ft_isalpha.c \
                          sources/ft_is/ft_isspace.c \
                          sources/ft_is/ft_isalnum.c \
                          sources/ft_is/ft_isascii.c \
                          sources/ft_is/ft_isdelimiter.c \
                          sources/ft_is/ft_isdigit.c \
                          sources/ft_is/ft_isprint.c \
                          sources/ft_lst/ft_lstadd_back.c \
                          sources/ft_lst/ft_lstadd_front.c \
                          sources/ft_lst/ft_lstclear.c \
                          sources/ft_lst/ft_lstdelone.c \
                          sources/ft_lst/ft_lstiter.c \
                          sources/ft_lst/ft_lstlast.c \
                          sources/ft_lst/ft_lstmap.c \
                          sources/ft_lst/ft_lstnew.c \
                          sources/ft_lst/ft_lstsize.c \
                          sources/ft_mem/ft_memchr.c \
                          sources/ft_mem/ft_memcmp.c \
                          sources/ft_mem/ft_memcpy.c \
                          sources/ft_mem/ft_memmove.c \
                          sources/ft_mem/ft_memset.c \
                          sources/ft_mem/ft_calloc.c \
                          sources/ft_mem/ft_bzero.c \

OBJS	=	${SRCS:.c=.o}

CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror -I includes -g
RM		=	rm -f
AR		=	ar rcs

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

all:	${NAME}

${NAME}:	${OBJS}
			${AR} ${NAME} ${OBJS}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
