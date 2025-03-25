#ifndef MINISHELL_H
# define MINISHELL_H

/*LIBRAIRIES*/

#include "../libft/libft.h"
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

/*MACROS*/

# define MAX_NB_CMDS 100
# define CNTRL_C SIGINT
# define CNTRL_B_SLASH SIGQUIT

/* structure qui stocke les infos liees au parsing selon la ligne de cmd entree */
typedef struct s_parsing
{
	char    **cmd;
	char	*line;
	char    *entry;
	char    *exit;
	char    *error_msg;
	char	**limiter;
	char	*heredoc_data; // stocke le contenu d'un heredoc
	int		is_append_mode;
	int     error_code;
}           t_parsing;

/* structure qui gere les pipes */
typedef struct s_pipes
{
	int pipefd[2]; // stocke 2 fd -> entrée du pipe [0] (lecture) & sortie du pipe [1] (ecriture)
}              t_pipes;

/* structure qui analyse et valide le chemin d'une commande */
typedef struct s_check_path
{
	char		*dir_or_f;
	int			counter;
	int			last;
	int			end_of_first;
	int			first;
	struct stat	sb;
}	t_check_path;

/* structure qui represente le contexte du shell et son etat */
typedef struct s_minishell
{
	char        **envp;
	char	    path[PATH_MAX]; // pwd[PATH_MAX]4096
	int         nb_of_cmds; // nb_cmds
	int         exit_code;
	t_parsing   *parsed; // all_parsed ?
	t_pipes     *pipes;
	pid_t       *pids;
}              t_minishell;

typedef int t_fd;

typedef struct s_extremes
{
	int	*j;
	int	*i;
}	t_extremes;

typedef struct s_line_n_ind
{
	char	*line;
	int		*i;
}	t_line_n_ind;

/*************** EXEC ***************/

/***** BUILTINS ******/

// builtins_utils
char		*ft_get_env(const char **env, const char *key);
int			ft_unset_env(char **env, const char *key);
int			ft_set_env(char ***env, const char *var, const char *value, int overwrite);
void    	write_error_msg(const char *title, const char *error_msg);

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
void		put_cursors(char *exp, int end, int *first, int *last); // a retirer
void		next_dir(char *exp, int *last, int end); // a retirer
void		cmd_path_error(const char *msg, char *path, t_parsing *parsing_data, int code);
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

void    	malloc_failure(void *str);
void    	free_words(char **list);
void    	free_all_parsed(t_parsing *all_parsed, int nb_cmds);

int 		ft_is_empty(char *str);
void 		write_prompt(t_minishell *minishell);
void		handle_sigint(int sig);
int 		main(int argc, char **argv, char **envp);

// prepare_cmds
int count_chars(char *str, char c);
// void    init_cmd_data(t_minishell *minishell, char *input);
// void    find_cmd_end(int *j, char *input);
// void    split_and_parse_cmd(t_minishell *minishell, char *input);
// void    prepare_cmds(char *input, t_minishell *minishell);
void	parsed_init(t_minishell *minishell, char *line);
void	one_cmd_end(int *j, char *line);
void	no_synt_error(t_minishell *minishell, char *line);
void	obtain_cmds(char *line, t_minishell *minishell);

/***** PARSING *****/
void	skip_spaces(int *i, char *line);
void	quote_to_quote(int *j, char *line, char quote);
void	another_end(char *line, int *j);
int	end_of_word(int *i, char *line);

int	delim(char *line, int strt, t_parsing *parsed, int *lim_size);
int	entry(char *line, int strt, t_parsing *parsed, t_minishell *minishell);
void	alloc_or_free(int on_sign, char *line, t_parsing *parsed, t_minishell *minishell);
int	ext(char *line, int strt, t_parsing *parsed, t_minishell *minishell);

char	*detec_signs(char *line, int *i, t_parsing *parsed);
void	detec_redir(t_line_n_ind lni, int *lim_size, t_parsing *parsed, t_minishell *minishell);
void	add_cmd(t_line_n_ind lni, int *cmd_size, t_parsing *parsed, t_minishell *minishell);
void	parsing(t_parsing *parsed, char *line, t_minishell *minishell, int init);

void	entry_error(char *entry, t_parsing *parsed, t_minishell *minishell);
void	open_file(char *exp, t_parsing *parsed);
void	exit_error(char *ex, t_parsing *parsed, t_minishell *minishell);

void	free_redir(t_parsing *parsed);
void	define_exp(char **dst, char *src, t_minishell *minishell);
void	found_error(t_parsing *parsed, char *exp, char *dir_or_f, const char *msg);
int	last_word(int first, int last);

int	not_readable(char **exp, t_parsing *parsed);
int	a_file(int end, char *exp, t_parsing *parsed, t_check_path *cp);
int	cannot_open(char *exp, t_parsing *parsed, t_check_path *cp);
int	path_error_en(char *exp, t_parsing *parsed);

void	put_cursors(char *exp, int end, int *first, int *last);
void	next_dir(char *exp, int *last, int end);

void	check_path_init(int *end, char *exp, t_check_path *cp);
int	wrong_data_type(t_check_path *cp, t_parsing *parsed, char *exp, int end);
int	not_writable(t_check_path *cp, t_parsing *parsed, char *exp, int end_of_file);

int	amb_redir(char *src, char *expanded, t_parsing *parsed);
void	path_without_pts(char **no_points, char *exp, int end);
int	always_a_dir(char *exp, int end, t_parsing *parsed);
int	bad_redir_gd_file(char *exp, t_parsing *parsed, struct stat sb);
int	path_error_ex(int end_of_path, char *exp, t_parsing *parsed);

void	at_last_dir(int *first, char *exp);
void	end_of_dir(char *exp, int *last);

void	add_str(char ***src, char *new, int *size);
char	*build_substring(char *line, unsigned int end, char **dst);

char	*reduce_str(char *str, int strt, int end);
char	*no_quotes(char *str);
void	dequote_parsed(t_parsing *parsed);

void	erase_var(char *line, int *i, int to_dol, int *limit);
void	new_limit(char *line, char *var_value, int *i, int *limit);
void	key_to_value(char *var_value, char **line, int key_len, int to_dol);

void	expand_nbr(char **line, t_extremes ex, int *limit, t_minishell *minishell);
void	expand_word(char **line, t_extremes ex, int *limit, t_minishell *minishell);
void	search_var(char **line, int *limit, int *strt, t_minishell *minishell);
void	expand(char **line, t_minishell *minishell);

void	find_quotes(char *str, char to_find, char *to_free);
void	opened_quotes(char *str, int times, char *to_free);

int	pipe_first(char *line, t_parsing *parsed);
void	opened_pipe(char *str);

void	before_pipe(t_parsing *parsed, char *line, int i);
void	before_entry(t_parsing *parsed, char *line, int i);
void	before_exit(t_parsing *parsed, char *line, int i);
void	before_ampersand(t_parsing *parsed, char *line, int i);

void	also_first(char frst, char *msg, t_parsing *parsed);
void	also_frst_n_scnd(char frst, char scnd, char *msg, t_parsing *parsed);
char	*put_all_letters(char frst, char scnd, char thrd);
void	syntax_err_msg(t_parsing *parsed, char first, char scnd, char third);

char	*limiters(char *line, int *i, t_parsing *parsed);
void	add_lim(char *buffer, int *lim_size, t_parsing *parsed);
void	entries_n_exits(char *line, int *i, t_parsing *parsed);
void	pipes(char *line, int *i, t_parsing *parsed);
void	syntax_error(char *line, t_parsing *parsed);

char *ft_strndup(const char *s, size_t n);




#endif






