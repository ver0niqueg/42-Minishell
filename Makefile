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

NAME =			minishell1
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
			$(SRCS_DIR)builtins.c \
			$(SRCS_DIR)end_program.c \
			$(SRCS_DIR)prepare_cmds.c \
			$(SRCS_DIR)main.c \

OBJS =			$(patsubst $(SRCS_DIR)%, $(OBJS_DIR)%, $(SRCS:.c=.o))

################################################################################
#                                     RULES                                    #
################################################################################

# Rule for compiling source files into object files
$(OBJS_DIR)%.o:		$(SRCS_DIR)%.c
					@mkdir -p $(dir $@)
					@$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDES_DIR)

# Rule for creating the executable
$(NAME):	$(OBJS)
			@make --no-print-directory -C $(MYLIB_DIR)
			@echo "$(GREEN)Linking $(YELLOW)$(NAME) $(CYAN)executable$(DEFAULT)..."
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

# Full clean rule (objects files, executable and libraries)
fclean:			
				@make fclean --no-print-directory -C $(MYLIB_DIR)
				@echo "$(RED)Deleting $(CYAN)project object files $(DEFAULT)..."
				@$(RM) -r $(OBJS_DIR)
				@echo "$(RED)Deleting $(YELLOW)$(NAME) $(CYAN)executable $(DEFAULT)..."
				@$(RM) $(NAME)
				@echo "$(GREEN)Done $(DEFAULT)✔️"

# Rebuild rule
re:				fclean all

# Rule to ensure that these targets are always executed as intended, even if there are files with the same name
.PHONY:			all clean fclean re