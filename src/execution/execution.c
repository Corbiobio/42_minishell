/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:10:01 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/29 17:07:38 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"
#include "../../includes/execution.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h> 
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

static t_position	get_pos(t_cmd_list *list, size_t curr_cmd_index)
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

void	free_exit_error_exec(t_cmd_list *list, char *path, int *status, size_t i)
{
	struct stat sb;

	*status = 126;
	write(STDERR_FILENO, "minishell: ", 12);
	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		write(STDERR_FILENO, path, ft_strlen(path));
		write(STDERR_FILENO, " is a directory\n", 17);
	}
	else if (access(path, F_OK) == -1)
	{
		write(STDERR_FILENO, list->cmds[i].cmd[0], ft_strlen(list->cmds[i].cmd[0]));
		if (ft_strchr(list->cmds[i].cmd[0], '/') != NULL)
			write(STDERR_FILENO, " no such file or directory\n", 28);
		else
			write(STDERR_FILENO, " command not found\n", 20);
		*status = 127;
	}
	else if (access(path, X_OK) == -1)
	{
		write(STDERR_FILENO, list->cmds[i].cmd[0], ft_strlen(list->cmds[i].cmd[0]));
		write(STDERR_FILENO, " Permission denied\n", 20);
	}
	free_cmd_list(list);
	free(path);
	exit(*status);
}

void	exec_cmd(int fds[3], t_cmd cmd, t_position pos, t_hash_table *env, t_cmd_list *list, size_t i, int *status)
{
	const char	**envp = (const char **)get_env_from_table(env);
	char		*path;

	infile_redirection(cmd, pos, fds);
	outfile_redirection(cmd, pos, fds);
	close_all_unused_io(list, i);
	if (launch_builtin(cmd, env, status, pos) == 1)
	{
		ft_free_split((char **)envp);
		free_cmd_list(list);
		table_delete_table(env);
		exit(*status);
	}
	path = get_cmd_path(cmd, env);
	table_delete_table(env);
	if (path != NULL && access(path, X_OK) == 0)
	{
		if (execve(path, cmd.cmd, (char **)envp) == -1)
			dprintf(2, "cannot exec %s\n", cmd.cmd[0]);
	}
	ft_free_split((char **)envp);
	if (path == NULL)
		exit(EXIT_FAILURE);
	free_exit_error_exec(list, path, status, i);
}

size_t count_cmds_with_correct_io(t_cmd_list *list)
{
	size_t count;
	size_t i;

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

int	calc_correct_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	printf("test\n");
	if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (status);
}

int	create_child_and_exec_cmd(t_cmd_list *list, t_hash_table *env, struct termios old_termios)
{
	t_position	pos;
	size_t		i;
	int			fds[3];
	pid_t		pid;
	int			status;

	set_signal_handler_exec(old_termios);
	i = 0;
	status = 0;
	while (i < list->nb_cmd)
	{
		pos = get_pos(list, i);
		if (pos == ALONE && launch_builtin(list->cmds[i], env, &status, pos) == 1)
			break ;
		if (pos != LAST && pos != ALONE && pipe(fds) == -1)
			dprintf(2, "cannot pipe on %s\n", list->cmds[i].cmd[0]);
		if (list->cmds[i].io[0] != -1 && list->cmds[i].io[1] != -1)
		{
			pid = fork();
			if (pid < 0)
				dprintf(2, "cannot fork on %s\n", list->cmds[i].cmd[0]);
			else if (pid == 0)
				exec_cmd(fds, list->cmds[i], pos, env, list, i, &status);
		}
		if (pos != FIRST && pos != ALONE)
			close(fds[2]);
		if (pos != LAST && pos != ALONE)
		{
			close(fds[1]);
			fds[2] = fds[0];
		}
		i++;
	}
	if ((pos != ALONE || (pos == ALONE && is_builtin(list->cmds[0]) == 0)) && count_cmds_with_correct_io(list) >= 1)
	{
		waitpid(pid, &status, 0);
		while (wait(0) > 0)
			;
	}
	if (count_cmds_with_correct_io(list) >= 1)
		table_insert(env, ft_strdup("?"), ft_itoa(calc_correct_status(status)));
	close_all_io(list);
	return (status);
}
