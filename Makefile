DEFAULT			= \033[0m
RED				= \033[1;31m
GREEN			= \033[1;32m
YELLOW			= \033[1;33m
MAGENTA			= \033[1;35m
CYAN			= \033[1;36m
WHITE			= \033[1;37m

################################################################################
#                                     CONFIG                                   #
################################################################################

NAME =			minishell
CC =			cc
CFLAGS =		-Wall -Wextra -Werror -g3
AR =			ar rcs
RM =			rm -f

################################################################################
#                                PROGRAM'S SOURCES                             #
################################################################################

# Directory paths
SRCS_DIR =		./sources/
OBJS_DIR =		./objets/
MYLIB_DIR =		./libft/
INCLUDES_DIR =	./includes/

#Libraries

SRCS =		$(SRCS_DIR)/builtins/builtins_utils.c \
			$(SRCS_DIR)/builtins/ft_cd.c \
			$(SRCS_DIR)/builtins/ft_echo.c \
			$(SRCS_DIR)/builtins/ft_env.c \
			$(SRCS_DIR)/builtins/ft_exit.c \
			$(SRCS_DIR)/builtins/ft_export.c \
			$(SRCS_DIR)/builtins/ft_pwd.c \
			$(SRCS_DIR)/builtins/ft_unset.c \
			$(SRCS_DIR)/execution/builtins.c \
			$(SRCS_DIR)/execution/child_processes/check_cmd_path.c \
			$(SRCS_DIR)/execution/child_processes/handle_process.c \
			$(SRCS_DIR)/execution/child_processes/set_input_redirs.c \
			$(SRCS_DIR)/execution/child_processes/set_output_redirs.c \
			$(SRCS_DIR)/execution/close_and_free.c \
			$(SRCS_DIR)/execution/create_pipes.c \
			$(SRCS_DIR)/execution/exec_cmds.c \
			$(SRCS_DIR)/execution/exec_utils.c \
			$(SRCS_DIR)/execution/handle_errors.c \
			$(SRCS_DIR)/execution/handle_limiters.c \
			$(SRCS_DIR)/parsing/error/error_msg.c \
			$(SRCS_DIR)/parsing/error/error_parsing.c \
			$(SRCS_DIR)/parsing/error/syntax_err_msg.c \
			$(SRCS_DIR)/parsing/redir/entry/find_dir_en.c \
			$(SRCS_DIR)/parsing/redir/entry/redirections_test.c \
			$(SRCS_DIR)/parsing/redir/exit/check_path_utils.c \
			$(SRCS_DIR)/parsing/redir/exit/ex_redir_test.c \
			$(SRCS_DIR)/parsing/redir/exit/find_dir_ex.c \
			$(SRCS_DIR)/parsing/redir/error_redirections.c \
			$(SRCS_DIR)/parsing/redir/redir_utils.c \
			$(SRCS_DIR)/parsing/redir/redirections.c \
			$(SRCS_DIR)/parsing/utils/errors_utils.c \
			$(SRCS_DIR)/parsing/utils/expand_utils.c \
			$(SRCS_DIR)/parsing/utils/free_utils.c \
			$(SRCS_DIR)/parsing/utils/general_utils.c \
			$(SRCS_DIR)/parsing/utils/quotes_utils.c \
			$(SRCS_DIR)/parsing/utils/string_utils.c \
			$(SRCS_DIR)/parsing/expand.c \
			$(SRCS_DIR)/parsing/no_syntax_error.c \
			$(SRCS_DIR)/parsing/parsing.c \
			$(SRCS_DIR)/parsing/quotes.c \
			$(SRCS_DIR)/parsing/syntax_error.c \
			$(SRCS_DIR)/parsing/understand_line.c \
			$(SRCS_DIR)main.c \

OBJS =			$(patsubst $(SRCS_DIR)%, $(OBJS_DIR)%, $(SRCS:.c=.o))

################################################################################
#                                     RULES                                    #
################################################################################

# Rule for compiling source files into object files
$(OBJS_DIR)%.o:		$(SRCS_DIR)%.c
					@mkdir -p $(dir $@)
					@$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDES_DIR)

# Rule for creating the executionutable
$(NAME):	$(OBJS)
			@make --no-print-directory -C $(MYLIB_DIR)
			@echo "$(GREEN)Linking $(YELLOW)$(NAME) $(CYAN)executionutable$(DEFAULT)..."
			$(CC) $(CFLAGS) $(OBJS) -L$(MYLIB_DIR) -lft -lreadline -o $(NAME)
			@echo "$(GREEN)Done $(DEFAULT)✔️"


# Default rule
all:			$(NAME)

# Rule for cleaning up object files
clean:
				@make clean --no-print-directory -C $(MYLIB_DIR)
				@echo "$(RED)Deleting $(CYAN)project object files $(DEFAULT)..."
				@$(RM) -r $(OBJS_DIR)
				@echo "$(GREEN)Done $(DEFAULT)✔️"

# Full clean rule (objects files, executionutable and libraries)
fclean:			
				@make fclean --no-print-directory -C $(MYLIB_DIR)
				@echo "$(RED)Deleting $(CYAN)project object files $(DEFAULT)..."
				@$(RM) -r $(OBJS_DIR)
				@echo "$(RED)Deleting $(YELLOW)$(NAME) $(CYAN)executionutable $(DEFAULT)..."
				@$(RM) $(NAME)
				@echo "$(GREEN)Done $(DEFAULT)✔️"

# Rebuild rule
re:				fclean all

# Rule to ensure that these targets are always executionuted as intended, even if there are files with the same name
.PHONY:			all clean fclean re