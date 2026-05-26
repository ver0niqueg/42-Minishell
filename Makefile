################################################################################
# COLORS
################################################################################

DEFAULT = \033[0m
GREEN	= \033[1;32m
RED		= \033[1;31m

################################################################################
# CONFIG
################################################################################

NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Werror -Wextra -g3

RM			= rm -f

LIBFT_DIR	= ./libft/
INCLUDES	= -I./includes/
LIBFT		= $(LIBFT_DIR)libft.a

SRCS = \
	sources/main.c \
	sources/builtins/builtins_utils.c \
	sources/builtins/ft_cd.c \
	sources/builtins/ft_echo.c \
	sources/builtins/ft_env.c \
	sources/builtins/ft_exit.c \
	sources/builtins/ft_export.c \
	sources/builtins/ft_pwd.c \
	sources/builtins/ft_unset.c \
	sources/execution/builtins.c \
	sources/execution/close_and_free.c \
	sources/execution/create_pipes.c \
	sources/execution/exec_cmds.c \
	sources/execution/exec_utils.c \
	sources/execution/handle_errors.c \
	sources/execution/handle_limiters.c \
	sources/execution/child_processes/check_cmd_path.c \
	sources/execution/child_processes/handle_process.c \
	sources/execution/child_processes/set_input_redirs.c \
	sources/execution/child_processes/set_output_redirs.c \
	sources/parsing/expand.c \
	sources/parsing/no_syntax_error.c \
	sources/parsing/parsing.c \
	sources/parsing/quotes.c \
	sources/parsing/syntax_error.c \
	sources/parsing/understand_line.c \
	sources/parsing/error/error_msg.c \
	sources/parsing/error/error_parsing.c \
	sources/parsing/error/syntax_err_msg.c \
	sources/parsing/redir/error_redirections.c \
	sources/parsing/redir/redir_utils.c \
	sources/parsing/redir/redirections.c \
	sources/parsing/redir/entry/find_dir_en.c \
	sources/parsing/redir/entry/redirections_test.c \
	sources/parsing/redir/exit/check_path_utils.c \
	sources/parsing/redir/exit/ex_redir_test.c \
	sources/parsing/redir/exit/find_dir_ex.c \
	sources/parsing/utils/errors_utils.c \
	sources/parsing/utils/expand_utils.c \
	sources/parsing/utils/free_utils.c \
	sources/parsing/utils/general_utils.c \
	sources/parsing/utils/quotes_utils.c \
	sources/parsing/utils/string_utils.c

OBJS = $(SRCS:.c=.o)

################################################################################
# RULES
################################################################################

all: $(NAME)

################################################################################
# MINISHELL
################################################################################

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
	@printf "$(GREEN)[OK]$(DEFAULT) minishell compiled\n"

$(LIBFT):
	@make --no-print-directory -C $(LIBFT_DIR)

################################################################################
# OBJECTS
################################################################################

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

################################################################################
# CLEAN
################################################################################

clean:
	@$(RM) $(OBJS)
	@make clean --no-print-directory -C $(LIBFT_DIR)
	@printf "$(RED)[CLEAN]$(DEFAULT) object files removed\n"

fclean:
	@$(RM) $(OBJS)
	@$(RM) $(NAME)
	@make fclean --no-print-directory -C $(LIBFT_DIR)
	@printf "$(RED)[FCLEAN]$(DEFAULT) executable removed\n"

re: fclean all

.PHONY: all clean fclean re