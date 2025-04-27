/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:10:01 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/25 16:21:59 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"
#include "../../includes/execution.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <fcntl.h>
#include <sys/wait.h>

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

static t_position	get_pos(t_cmd_list *list, size_t curr_cmd_index)
{
	if (curr_cmd_index == list->nb_cmd - 1)
		return (LAST);
	else if (curr_cmd_index == 0)
		return (FIRST);
	else
		return (MID);
}

void	infile_redirection(t_cmd cmd, t_position pos, int fds[3])
{
	if (cmd.io[0] >= 0)
	{
		dup2(cmd.io[0], STDIN_FILENO);
		close(cmd.io[0]);
	}
	else if (pos != FIRST)
		dup2(fds[2], STDIN_FILENO);
	if (fds[2] != -1)
		close(fds[2]);
}

void	outfile_redirection(t_cmd cmd, t_position pos, int fds[3])
{
	if (cmd.io[1] >= 0)
	{
		dup2(cmd.io[1], STDOUT_FILENO);
		close(cmd.io[1]);
	}
	else if (pos != LAST)
		dup2(fds[1], STDOUT_FILENO);
	if (fds[1] != -1)
		close(fds[1]);
	if (fds[1] != -1)
		close(fds[0]);
}

int	exec_cmd(int fds[3], t_cmd cmd, t_position pos, t_hash_table *env, t_cmd_list *list, size_t i)
{
	char	*path;

	if (cmd.io[0] >= 0 || pos != FIRST)
		infile_redirection(cmd, pos, fds);
	if (cmd.io[1] >= 0 || pos != LAST)
		outfile_redirection(cmd, pos, fds);
	close_all_unused_io(list, i);
	path = get_cmd_path(cmd, env);
	table_delete_table(env);
	if (path != NULL && access(path, X_OK) == 0)
	{
		if (execve(path, cmd.cmd, NULL) == -1)
			dprintf(2, "cannot exec %s\n", cmd.cmd[0]);
	}
	else
		dprintf(2, "cannot find correct path for %s\n", cmd.cmd[0]);
	free(path);
	exit (EXIT_FAILURE);
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

void	create_child_and_exec_cmd(t_cmd_list *list, t_hash_table *env)
{
	t_position	pos;
	size_t		i;
	int			fds[3];
	int			pid;

	if (list->nb_cmd == 1 && list->cmds[0].cmd[0] == NULL)
		return ;
	i = 0;
	fds[0] = -1;
	fds[1] = -1;
	fds[2] = -1;
	while (i < list->nb_cmd)
	{
		pos = get_pos(list, i);
		if (pos != LAST && pipe(fds) == -1)
			dprintf(2, "cannot pipe on %s\n", list->cmds[i].cmd[0]);
		pid = fork();
		if (pid < 0)
			dprintf(2, "cannot fork on %s\n", list->cmds[i].cmd[0]);
		else if (pid == 0)
			exec_cmd(fds, list->cmds[i], pos, env, list, i);
		if (fds[2] != -1)
			close(fds[2]);
		if (pos != LAST)
		{
			if (fds[1] != -1)
				close(fds[1]);
			fds[2] = fds[0];
		}
		i++;
	}
	close_all_io(list);
}
