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

SRCS =		$(SRCS_DIR)/error/error_msg.c \
			$(SRCS_DIR)/error/error_parsing.c \
			$(SRCS_DIR)/error/syntax_err_msg.c \
			$(SRCS_DIR)/redir/check_path_utils.c \
			$(SRCS_DIR)/redir/error_redirections.c \
			$(SRCS_DIR)/redir/ex_redir_test.c \
			$(SRCS_DIR)/redir/find_dir_en.c \
			$(SRCS_DIR)/redir/find_dir_ex.c \
			$(SRCS_DIR)/redir/redir_utils.c \
			$(SRCS_DIR)/redir/redirections_test.c \
			$(SRCS_DIR)/redir/redirections.c \
			$(SRCS_DIR)/utils/errors_utils.c \
			$(SRCS_DIR)/utils/expand_utils.c \
			$(SRCS_DIR)/utils/free_utils.c \
			$(SRCS_DIR)/utils/general_utils.c \
			$(SRCS_DIR)/utils/quotes_utils.c \
			$(SRCS_DIR)/utils/string_utils.c \
			$(SRCS_DIR)expand.c \
			$(SRCS_DIR)main.c \
			$(SRCS_DIR)no_syntax_error.c \
			$(SRCS_DIR)parsing.c \
			$(SRCS_DIR)quotes.c \
			$(SRCS_DIR)request_limiters.c \
			$(SRCS_DIR)syntax_error.c \
			$(SRCS_DIR)understand_line.c \

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