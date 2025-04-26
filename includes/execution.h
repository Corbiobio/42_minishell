/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:55:15 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/26 17:00:19 by sflechel         ###   ########.fr       */
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
	LAST
}	t_position;

//path
char	*get_cmd_path(t_cmd cmd, t_hash_table *env);

//execution
//void	create_child_and_exec_cmd(t_cmd_list *list, t_hash_table *env);
void	create_child_and_exec_cmd(t_cmd_list *list, t_hash_table *env, struct termios old_termios);

//echo
void	echo(t_cmd cmd);

//signal_exec
// void	set_signal_handler_exec(void);
void	set_signal_handler_exec(struct termios old_termios);

//env
char	**get_env_from_table(t_hash_table *env_table);

#endif
