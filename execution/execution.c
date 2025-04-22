/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:10:01 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/22 13:32:31 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"
#include <stdlib.h>
#include <unistd.h> 
#include <fcntl.h>
#include <sys/wait.h>

static t_position	get_pos(t_cmd_list *list, size_t index)
{
	if (index == list->nb_cmd - 1)
		return (LAST);
	else if (index == 0)
		return (FIRST);
	else
		return (MID);
}

void	exec_cmd(int fds[3], t_cmd cmd, t_position pos, char **env)
{
	if (cmd.io[0] >= 0)//including std ??
		dup2(cmd.io[0], STDIN_FILENO);
	else if (pos != FIRST)
	{
		dup2(fds[2], STDIN_FILENO);
		close(fds[2]);
	}
	if (cmd.io[1] >= 0)//including std ??
		dup2(cmd.io[1], STDOUT_FILENO);
	else if (pos != LAST)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		close(fds[0]);
	}
	if (access(cmd.cmd[0], X_OK) >= 0)
	{
		if (execve(cmd.cmd[0], cmd.cmd, env) == -1)
			write(STDERR_FILENO, "errooooor", 10);
	}
	write(STDERR_FILENO, "errooooor", 10);
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
			write(STDERR_FILENO, "errooooor", 10);
		pid = fork();
		if (pid < 0)
			write(STDERR_FILENO, "errooooor", 10);
		else if (pid == 0)
			exec_cmd(fds, list->cmds[i], pos, env);
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
//	int	status;
//	t_cmd_list	*cmd_list = malloc(sizeof(t_cmd_list) + 5 * sizeof(t_cmd));

//	int in = open("in", O_RDWR);
//	int out = open("out", O_RDWR);
//	cmd_list->cmds[0] = (t_cmd){ {-2, out},(char*[]){"/usr/bin/echo", "test",NULL}};
//	cmd_list->cmds[1] = (t_cmd){ {out, -2},(char*[]){"/usr/bin/cat", "-e",NULL}};
//	cmd_list->cmds[2] = (t_cmd){ {-2, -2},(char*[]){"/usr/bin/wc", "-m",NULL}};
//	//cmd_list->cmds[0] = (t_cmd){ {-2, -2},(char*[]){"/usr/bin/echo", "ceci est un test",NULL}};
//	//cmd_list->cmds[1] = (t_cmd){ {-2, -2},(char*[]){"/usr/bin/cat", "-e", NULL}};
//	//cmd_list->cmds[2] = (t_cmd){ {-2, -2},(char*[]){"/usr/bin/cat", "-e", NULL}};
//	//cmd_list->cmds[3] = (t_cmd){ {-2, -2},(char*[]){"/usr/bin/cat", "-e", NULL}};
//	//cmd_list->cmds[4] = (t_cmd){ {-2, -2},(char*[]){"/usr/bin/cat", "-e", NULL}};
//	cmd_list->nb_cmd = 3;

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
