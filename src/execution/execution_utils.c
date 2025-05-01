/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:09:34 by edarnand          #+#    #+#             */
/*   Updated: 2025/05/01 19:11:56 by edarnand         ###   ########.fr       */
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
		return (WTERMSIG(status) + 128);
	return (status);
}
