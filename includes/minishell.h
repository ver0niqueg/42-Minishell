/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <viviane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 12:21:08 by viviane           #+#    #+#             */
/*   Updated: 2025/03/24 21:35:34 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

/*LIBRAIRIES*/

#include "../libft/includes/libft.h"
#include "../libft/includes/get_next_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include <termios.h>
#include <curses.h>
#include <dirent.h>

/*MACRO*/

# define NB_MAX_OF_CMDS 511

/*Structure aui stockent les informations lies au parsing selon la ligne de commande entree*/
typedef struct s_parsing
{
    char **cmd;
    char *line;
    char *exit;
    char *entry;
    char **limiter;
    char *err_msg;
    int  err_nb;
    int  ex_append;
    char *limiter_data;
}   t_parsing;

/*Structure qui  represente le contexte du shell et/son etat*/
typedef struct s_minishell
{
    char        **envp;
    char        path[4096];
    int         nb_of_cmds;
    int         ex_nb;
    t_parsing   *parsed;
}          t_minishell;

typedef struct s_line_info
{
    char    *line;
    int     *index;
}   t_line_info;

typedef struct s_pointers
{
    int *i;
    int *j;
}   t_point;

typedef struct s_check_path
{
	char		*dir_or_fil;
	int			counter;
	int			last;
	int			end_of_first;
	int			first;
	struct stat	sb;
}	t_check_path;

void    before_pipe(t_parsing *parsed, char *line, int i);
void    before_entry(t_parsing *parsed, char *line, int i);
void    before_exit(t_parsing *parsed, char *line, int i);
void    before_esper(t_parsing *parsed, char *line, int i);

void    if_syntax_error(char *line, t_parsing *parsed);

void    entry_error(char *entry, t_parsing *parsed, t_minishell *minishell);
void    exit_error(char *exit, t_parsing *parsed, t_minishell *minishell);

void    also_first(char first, char *msg, t_parsing *parsed);
void    also_first_and_second(char first, char second, char *msg, t_parsing *parsed);
char    *put_all_letters(char first, char second, char third);
void    syntax_err_msg(t_parsing *parsed, char first, char second, char third);

void    is_failed_malloc(char *str);
void    print_error(char *str);

void    delete_var(char *line, int *i, int to_dollar, int *new_len);
void    new_lenght(char *line, char *value, int *i, int *new_len);
void    change_to_value(char *value, char **line, int key_len, int to_dollar);

void    free_words(char **words);
void    free_quotes(char *line);
void    free_parsed(t_parsing *parsed, int nb_of_cmds);
void    free_redir(t_parsing *parsed);
void    free_limiters(t_parsing *parsed, int nb_of_cmds);

int     if_pipe_first(char *line, t_parsing *minishell);
void    ft_skip_spaces(int *i, char *line);
int     ft_empty_line(char *line);

void   find_second_quote(int *i, char *line, char quote);
char   *erased_str(char *str, int i, int end);
char   *erase_quote(char *str);
void    clean_quotes(t_parsing *parsed);

void    open_file(char *exp, t_parsing *parsed);
void    alloc_or_free(int on_sign, char *line, t_parsing *parsed, t_minishell *minishell);
void    define_exp(char **dest, char *src, t_minishell *minishell);
void    found_error(t_parsing *parsed, char *exp, char *dir_or_fil, const char *msg);
int     last_word(int first, int last);

char   *reduce_line(char *line, int start, int end);
void   add_str(char ***src_str, char *new_str, int *len);
char   *ft_substring(char *line, unsigned int end, char **dest);
void   another_case_of_end(char *line, int *j);
int    find_end_of_word(int *i, char *line);

void   expand_word(char **line, t_point expand, int *new_len, t_minishell *minishell);
void   expand_nbr(char **line, t_point expand, int *new_len, t_minishell *minishell);
void   look_for_var(char **line, int *new_len, int *start, t_minishell *minishell);
void   expand(char **line, t_minishell *minishell);

int     request_limiters(t_parsing *p, int nb_of_cmds);
void    prompt(t_minishell *minishell);
int     main(int argc, char **argv, char **envp);

void    if_no_syntax_error(char *line, t_minishell *minishell);

void    handle_redir_type(t_line_info line_info, int *limiter_size, t_parsing *parsed, t_minishell *minishell);
char    *extract_token(char *line, int *i, t_parsing *parsed);
void    add_command(t_line_info line_info, int *cmd_size, t_parsing *parsed, t_minishell *minishell);
void    parsing(t_parsing *parsed, char *line, t_minishell *minishell, int init);

void    handle_quote(char *line, char quote, char *to_free);
void    handle_first_quote(char *line, int inside_quote, char *to_free);

int     handle_heredoc(char *line, int start, t_parsing *parsed, int *limiter_size);
int     handle_entry(char *line, int start, t_parsing *parsed, t_minishell *minishell);
int     handle_exit(char *line, int start, t_parsing *parsed, t_minishell *minishell);

char   *put_all(char first, char second, char third);
void   put_first_and_second(char first, char second, char *err_msg, t_parsing *parsed);
void   put_first(char first, char *err_msg, t_parsing *parsed);
void   syntax_err_msg(t_parsing *parsed, char first, char second, char third);

int     count_chars(char *line, char c);
void    parse_init_line(char *line, t_minishell *minishell);
void    find_one_cmd(int *i, char *line);
void    understand_line(char *line, t_minishell *minishell);

int     amb_redir(char *entry, char *exp, t_parsing *parsed);
void    path_without_pts(char **no_points, char *exp, int end);
int     always_a_dir(char *exp, int end, t_parsing *parsed);
int     bad_redir_gd_file(char *exp, t_parsing *parsed, struct stat sb);

int     not_readable(char **exp, t_parsing *parsed);
int     a_file(int end, char *exp, t_parsing *parsed, t_check_path *cp);
int     cannot_open(char *exp, t_parsing *parsed, t_check_path *cp);
int     path_error_en(char *exp, t_parsing *parsed);

void    entry_error(char *entry, t_parsing *parsed, t_minishell *minishell);
void    exit_error(char *exit, t_parsing *parsed, t_minishell *minishell);

int     not_readable(char **exp, t_parsing *parsed);
int     a_file(int end, char *exp, t_parsing *parsed, t_check_path *cp);
int     cannot_open(char *exp, t_parsing *parsed, t_check_path *cp);
int     path_error_ex(int end_of_path, char *exp, t_parsing *parsed);

void    check_path_init(int *end, char *exp, t_check_path *cp);
int     wrong_data_type(t_check_path *cp, t_parsing *parsed, char *exp, int end);
int     not_writable(t_check_path *cp, t_parsing *parsed, char *exp, int end_of_file);

void at_last_dir(int *first, char *exp);
void end_of_dir(char *exp, int *last);

void    put_cursors(char *exp, int end, int *first, int *last);
void    next_dir(char *exp, int *last, int end);

char    *ft_getenv(const char **envp, const char *name);

#endif