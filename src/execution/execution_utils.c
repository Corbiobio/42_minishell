/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:09:34 by edarnand          #+#    #+#             */
/*   Updated: 2025/05/09 15:11:38 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

t_position	get_pos(t_cmd_list *list, size_t curr_cmd_index)
{
	if (list->nb_cmd == 1)
		return (ALONE);
	else if (curr_cmd_index == list->nb_cmd - 1)
		return (LAST);
	else if (curr_cmd_index == 0)
		return (FIRST);
	else
		return (MID);
}

int	calc_correct_status(t_cmd_list *list, int status, int is_alone)
{
	if (is_alone && is_builtin(list->cmds[list->nb_cmd - 1]))
		return (status);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		return (WTERMSIG(status) + 128);
	}
	return (status);
}
