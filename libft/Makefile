################################################################################
# COLORS
################################################################################

DEFAULT = \033[0m
GREEN	= \033[1;32m
RED		= \033[1;31m

################################################################################
# CONFIG
################################################################################

NAME		= libft.a

CC			= cc
CFLAGS		= -Wall -Werror -Wextra -I includes -g

AR			= ar rcs
RM			= rm -f

SRCS = \
	sources/ft_str/ft_strchr.c \
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
	sources/ft_to/ft_ftoa.c \
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
	sources/ft_mem/ft_bzero.c

OBJS = $(SRCS:.c=.o)

################################################################################
# RULES
################################################################################

all: $(NAME)

################################################################################
# LIBFT
################################################################################

$(NAME): $(OBJS)
	@$(AR) $(NAME) $(OBJS)
	@printf "$(GREEN)[OK]$(DEFAULT) libft compiled\n"

################################################################################
# OBJECTS
################################################################################

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

################################################################################
# CLEAN
################################################################################

clean:
	@$(RM) $(OBJS)
	@printf "$(RED)[CLEAN]$(DEFAULT) object files removed\n"

fclean:
	@$(RM) $(OBJS)
	@$(RM) $(NAME)
	@printf "$(RED)[FCLEAN]$(DEFAULT) archive removed\n"

re: fclean all

.PHONY: all clean fclean re
