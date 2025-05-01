/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:55:15 by edarnand          #+#    #+#             */
/*   Updated: 2025/05/01 19:11:41 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include "libft.h"

typedef enum e_position
{
	FIRST,
	MID,
	LAST,
	ALONE
}	t_position;

typedef struct s_curr_cmd
{
	t_cmd		cmd;
	size_t		index;
	t_position	pos;
	int			fds[3];
}	t_curr_cmd;

//path
char		*get_cmd_path(t_cmd cmd, t_hash_table *env);

//execution
int			exec_cdms_list(t_cmd_list *list, t_hash_table *env,
				struct termios old_termios);

//execution_utils
t_position	get_pos(t_cmd_list *list, size_t curr_cmd_index);
int			calc_correct_status(t_cmd_list *list, int status, int is_alone);

//io_and_pipe
void		infile_redirection(t_curr_cmd curr_cmd);
void		outfile_redirection(t_curr_cmd curr_cmd);
void		close_all_unused_io(t_cmd_list *list, size_t curr_cmd_index);
void		close_all_io(t_cmd_list *list);
size_t		count_cmds_with_correct_io(t_cmd_list *list);

//builtin
int			launch_builtin(t_cmd cmd, t_hash_table *env, int *status,
				t_position pos);
int			is_builtin(t_cmd cmd);
void		set_stds_to_default(t_cmd cmd, t_position pos, int stds[2]);

//signal_exec
// void	set_signal_handler_exec(void);
void		set_signal_handler_exec(struct termios old_termios);

//env
char		**get_env_from_table(t_hash_table *env_table);

#endif
