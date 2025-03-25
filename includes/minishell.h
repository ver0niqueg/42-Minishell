/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <viviane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 12:21:08 by viviane           #+#    #+#             */
/*   Updated: 2025/03/25 18:28:42 by viviane          ###   ########.fr       */
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
#include <Kernel/sys/syslimits.h>

/*MACRO*/

# define NB_MAX_OF_CMDS 511
# define CNTRL_C SIGINT
# define CNTRL_B_SLASH SIGQUIT

/*Structure qui stockent les informations lies au parsing selon la ligne de commande entree*/
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

/* structure qui gere les pipes */
typedef struct s_pipes
{
	int pipefd[2]; // stocke 2 fd -> entrée du pipe [0] (lecture) & sortie du pipe [1] (ecriture)
}   t_pipes;

//Structure qui represente le contexte du shell et son etat

typedef struct s_minishell
{
    char        **envp;
    char        path[PATH_MAX];
    int         nb_of_cmds;
    int         exit_code;
    t_parsing   *parsed;
    t_pipes     *pipes;
    pid_t       *pids;
}          t_minishell;

typedef int t_fd;

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

/*************** PARSING ***************/

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

void    is_malloc_failed(char *str);
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

/*************** EXEC ***************/

/***** BUILTINS ******/

// builtins_utils
char		*ft_get_env(const char **env, const char *key);
int			ft_unset_env(char **env, const char *key);
int			ft_set_env(char ***env, const char *var, const char *value, int overwrite);
void    	write_error_msg(const char *title, const char *err_msg);

// builtins (cd, echo, env, exit, export, pwd, unset)
int			ft_cd(char **args, t_fd input_fd, t_fd output_fd, char ***envp);
int			ft_echo(char **args, t_fd input_fd, t_fd output_fd, char ***envp);
int			ft_env(char **args, t_fd input_fd, t_fd output_fd, char ***envp);
void		cd_write_error(const char *s1, const char *s2, const char *s3);
void		close_fds_and_exit(t_fd input_fd, t_fd output_fd, int return_code);
int			ft_exit(char **args, t_fd input_fd, t_fd output_fd, char ***envp);
int			ft_export(char **args, t_fd input_fd, t_fd output_fd, char ***envp);
int			ft_pwd(char **args, t_fd input_fd, t_fd output_fd, char ***envp);
int			ft_unset(char **args, t_fd input_fd, t_fd output_fd, char ***envp);

// exec_builtins
bool    	is_builtin(const char *cmd);
int 		exec_builtin(char **argv, t_fd input_fd, t_fd output_fd, char ***envp);

/***** CHILD PROCESSES ******/

// check_cmd_path

void		cmd_path_error(const char *msg, char *path, t_parsing *parsing_data, int nb);
int			validate_file_access(t_check_path *cp, char *path, t_parsing *parsing_data);
int			analyze_path_and_perms(t_check_path *cp, char *path, t_parsing *parsing_data);
void		validate_cmd_path(char *path, t_parsing *parsing_data);

// input_redirs
void		handle_heredoc_input(int i, t_minishell *minishell, t_pipes *pipes, pid_t *pids);
void		redirect_input_from_file(int *fd_in, char *entry);
void		redirect_input_from_pipe(int i, t_minishell *minishell, t_pipes *pipes, pid_t *pids);
int			set_input_redirs(int i, t_minishell *minishell, t_pipes *pipes, pid_t *pids);

// output_redirs
void		redirect_output_to_next_pipe(t_minishell *minishell, int i, t_pipes *pipes, pid_t *pids);
void		redirect_output_to_file(t_minishell *minishell, int i, int *fd_out);
int			set_output_redirs(int i, t_minishell *minishell, t_pipes *pipes, pid_t *pids);

// handle_processes
void		exec_cmd(int i, t_minishell *minishell, t_pipes *pipes, pid_t *pids);
void		cmd_error(int i, t_minishell *minishell, int *fd_out, int *fd_in);
int			cmd_null(int i, t_minishell *minishell);
void		end_of_process(t_minishell *minishell, t_pipes *pipes, pid_t *pids);
void		handle_cmd_exec(int i, t_minishell *minishell, t_pipes *pipes, pid_t *pids);

/***** CMD EXECUTION *****/

// create_pipes
void    	create_pipes(t_pipes *p, int nb_pipes);
void    	launch_processes(t_minishell *minishell, int nb_pipes);

// handle_limiters
int			handle_limiters(t_parsing *p, int nb_cmds);

// exec_cmds
void    	exec_one_builtin(t_minishell *minishell, int *builtin);
void    	exit_syntax_error(t_minishell *minishell);
void    	clean_exit(t_minishell *minishell, int fork_nb, int exit_code, int i);
int 		one_valid_cmd(t_minishell *minishell);
void		exec_commands(t_minishell *minishell, int nb_forks);

// close_and_free
void    	close_fd(int *fd);
void    	close_all_pipes(t_pipes *p, int nbr_of_cmds);
void    	free_split(char **strs);
void    	get_pid_status(int last_process, t_minishell *minishell);

// handle_errors
void		execve_error(char **args, t_minishell *minishell);
void		dup_error(int fd);
void		fork_error(t_pipes *p, int nbr_of_cmds);
void		pids_alloc_error(t_pipes *p, int nbr_of_cmds);
void		print_error(char *error);

// exec_utils
char    	**find_env_paths(char **env);
char    	**matching_path(char **args, char **env_paths);
char    	**is_executable(char **args, char **env_paths, char **env);

/***** PROGRAM ******/

int 		ft_empty_line(char *str);
void 		write_prompt(t_minishell *minishell);
void		handle_sigint(int sig);
int 		main(int argc, char **argv, char **envp);

#endif