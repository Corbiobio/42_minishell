/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:55:15 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/28 17:28:55 by sflechel         ###   ########.fr       */
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

//path
char	*get_cmd_path(t_cmd cmd, t_hash_table *env);

//execution
int		create_child_and_exec_cmd(t_cmd_list *list, t_hash_table *env, struct termios old_termios);

//io_and_pipe
void	infile_redirection(t_cmd cmd, t_position pos, int fds[3]);
void	outfile_redirection(t_cmd cmd, t_position pos, int fds[3]);
void	close_all_unused_io(t_cmd_list *list, size_t curr_cmd_index);
void	close_all_io(t_cmd_list *list);

//builtin
int		launch_builtin(t_cmd cmd, t_hash_table *env, int *status, t_position pos);
int		is_builtin(t_cmd cmd);

//signal_exec
// void	set_signal_handler_exec(void);
void	set_signal_handler_exec(struct termios old_termios);

//env
char	**get_env_from_table(t_hash_table *env_table);

#endif
