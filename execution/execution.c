/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:10:01 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/23 12:45:44 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <fcntl.h>
#include <sys/wait.h>

void	close_all_unused_fds(t_cmd_list *list, size_t curr_cmd_index)
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

void	exec_cmd(int fds[3], t_cmd cmd, t_position pos, char **env, t_cmd_list *list, size_t i)
{
	char	*path;

	if (cmd.io[0] >= 0)
	{
		dup2(cmd.io[0], STDIN_FILENO);
		close(cmd.io[0]);
		close(fds[2]);
	}
	else if (pos != FIRST)
	{
		dup2(fds[2], STDIN_FILENO);
		close(fds[2]);
	}
	if (cmd.io[1] >= 0)
	{
		dup2(cmd.io[1], STDOUT_FILENO);
		close(cmd.io[1]);
		close(fds[1]);
		close(fds[0]);
	}
	else if (pos != LAST)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		close(fds[0]);
	}
	close_all_unused_fds(list, i);
	(void)list;
	(void)i;
	path = get_cmd_path(cmd, env);
	if (path != NULL && access(path, X_OK) == 0)
	{
		if (execve(path, cmd.cmd, env) == -1)
			write(STDERR_FILENO, "cannot execve\n", 15);
	}
	else
		write(STDERR_FILENO, "cannot access to path\n", 23);
	free(path);
}

void	create_child_and_exec_cmd(t_cmd_list *list, char **env)
{
	t_position	pos;
	size_t		i;
	int			fds[3];
	int			pid;

	i = 0;
	while (i < list->nb_cmd)
	{
		pos = get_pos(list, i);
		if (pos != LAST && pipe(fds) == -1)
			write(STDERR_FILENO, "errooooor on pipe\n", 19);
		pid = fork();
		if (pid < 0)
			write(STDERR_FILENO, "errooooor on fork\n", 19);
		else if (pid == 0)
			exec_cmd(fds, list->cmds[i], pos, env, list, i);
		if (pos != FIRST)
			close(fds[2]);
		if (pos != LAST)
		{
			close(fds[1]);
			fds[2] = fds[0];
		}
		i++;
	}
}

//int	main(int ac, char **av, char **env)
//{
//	t_cmd_list	*cmd_list = malloc(sizeof(t_cmd_list) + 3 * sizeof(t_cmd));
//	int			status;

//	int in = open("in", O_RDWR);
//	int out = open("out", O_RDWR);
//	cmd_list->cmds[0] = (t_cmd){ {-2, out},(char*[]){"echo", "test",NULL}};
//	cmd_list->cmds[1] = (t_cmd){ {in, -2},(char*[]){"cat", "-e",NULL}};
//	//cmd_list->cmds[2] = (t_cmd){ {-2, -2},(char*[]){"/usr/bin/wc", "-m",NULL}};
//	//cmd_list->cmds[0] = (t_cmd){ {-2, -2},(char*[]){"/usr/bin/echo", "ceci est un test",NULL}};
//	//cmd_list->cmds[1] = (t_cmd){ {-2, -2},(char*[]){"/usr/bin/cat", "-e", NULL}};
//	//cmd_list->cmds[2] = (t_cmd){ {-2, -2},(char*[]){"/usr/bin/cat", "-e", NULL}};
//	//cmd_list->cmds[3] = (t_cmd){ {-2, -2},(char*[]){"/usr/bin/cat", "-e", NULL}};
//	//cmd_list->cmds[4] = (t_cmd){ {-2, -2},(char*[]){"/usr/bin/cat", "-e", NULL}};
//	cmd_list->nb_cmd = 2;
//	create_child_and_exec_cmd(cmd_list, env);
//	while (wait(&status) > 0)
//		;
//	free(cmd_list);
//	close(in);
//	close(out);
//	(void)ac;
//	(void)av;
//	return (status);
//}
