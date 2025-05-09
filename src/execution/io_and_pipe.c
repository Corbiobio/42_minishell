/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_and_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:34:24 by edarnand          #+#    #+#             */
/*   Updated: 2025/05/09 15:11:17 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	infile_redirection(t_curr_cmd curr_cmd)
{
	if (curr_cmd.cmd.io[0] >= 0)
	{
		dup2(curr_cmd.cmd.io[0], STDIN_FILENO);
		close(curr_cmd.cmd.io[0]);
	}
	else if (curr_cmd.pos != FIRST && curr_cmd.pos != ALONE)
		dup2(curr_cmd.fds[2], STDIN_FILENO);
	if (curr_cmd.pos != FIRST && curr_cmd.pos != ALONE)
		close(curr_cmd.fds[2]);
}

void	outfile_redirection(t_curr_cmd curr_cmd)
{
	if (curr_cmd.cmd.io[1] >= 0)
	{
		dup2(curr_cmd.cmd.io[1], STDOUT_FILENO);
		close(curr_cmd.cmd.io[1]);
	}
	else if (curr_cmd.pos != LAST && curr_cmd.pos != ALONE)
		dup2(curr_cmd.fds[1], STDOUT_FILENO);
	if (curr_cmd.pos != LAST && curr_cmd.pos != ALONE)
	{
		close(curr_cmd.fds[1]);
		close(curr_cmd.fds[0]);
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

size_t	count_cmds_with_correct_io(t_cmd_list *list)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (i < list->nb_cmd)
	{
		if (list->cmds[i].io[0] != -1 && list->cmds[i].io[1] != -1)
			count++;
		i++;
	}
	return (count);
}
