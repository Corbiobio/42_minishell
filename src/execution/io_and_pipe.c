/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_and_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:34:24 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/28 13:45:30 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <unistd.h>

void	infile_redirection(t_cmd cmd, t_position pos, int fds[3])
{
	if (cmd.io[0] >= 0)
	{
		dup2(cmd.io[0], STDIN_FILENO);
		close(cmd.io[0]);
	}
	else if (pos != FIRST && pos != ALONE)
		dup2(fds[2], STDIN_FILENO);
	if (pos != FIRST && pos != ALONE)
		close(fds[2]);
}

void	outfile_redirection(t_cmd cmd, t_position pos, int fds[3])
{
	if (cmd.io[1] >= 0)
	{
		dup2(cmd.io[1], STDOUT_FILENO);
		close(cmd.io[1]);
	}
	else if (pos != LAST && pos != ALONE)
		dup2(fds[1], STDOUT_FILENO);
	if (pos != LAST && pos != ALONE)
	{
		close(fds[1]);
		close(fds[0]);
	}
}

void	close_all_io(t_cmd_list *list)
{
	size_t	i;

	i = 0;
	while (i < list->nb_cmd)
	{
		if (list->cmds[i].io[0] >= 0)
			close(list->cmds[i].io[0]);
		if (list->cmds[i].io[1] >= 0)
			close(list->cmds[i].io[1]);
		i++;
	}
}

void	close_all_unused_io(t_cmd_list *list, size_t curr_cmd_index)
{
	size_t	i;

	i = 0;
	while (i < list->nb_cmd)
	{
		if (i != curr_cmd_index && list->cmds[i].io[0] >= 0)
			close(list->cmds[i].io[0]);
		if (i != curr_cmd_index && list->cmds[i].io[1] >= 0)
			close(list->cmds[i].io[1]);
		i++;
	}
}
