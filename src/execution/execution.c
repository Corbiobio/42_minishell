/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:10:01 by edarnand          #+#    #+#             */
/*   Updated: 2025/05/01 19:10:22 by edarnand         ###   ########.fr       */
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

void	free_exit_error_exec(t_cmd_list *list, int *status,
			size_t i, char *path)
{
	struct stat	sb;

	*status = 126;
	write(STDERR_FILENO, "minishell: ", 12);
	write(STDERR_FILENO, list->cmds[i].cmd[0], ft_strlen(list->cmds[i].cmd[0]));
	if (stat(list->cmds[i].cmd[0], &sb) == -1)
	{
		if (ft_strchr(list->cmds[i].cmd[0], '/') != NULL)
			write(STDERR_FILENO, " no such file or directory\n", 27);
		else
			write(STDERR_FILENO, " command not found\n", 19);
		*status = 127;
	}
	else if (path != NULL && S_ISDIR(sb.st_mode))
		write(STDERR_FILENO, " is a directory\n", 16);
	else if (path != NULL)
		write(STDERR_FILENO, " permission denied\n", 19);
	else
		write(STDERR_FILENO, " command not found\n", 19);
	free(path);
	free_cmd_list(list);
	exit(*status);
}

void	exec_cmd(t_cmd_list *list, t_hash_table *env, t_curr_cmd curr_cmd,
			int *status)
{
	const char	**envp = (const char **)get_env_from_table(env);
	const t_cmd	cmd_param = curr_cmd.cmd;
	char		*path;

	infile_redirection(curr_cmd);
	outfile_redirection(curr_cmd);
	close_all_unused_io(list, curr_cmd.index);
	if (curr_cmd.cmd.cmd[0] == NULL || is_builtin(cmd_param))
	{
		if (is_builtin(cmd_param))
			launch_builtin(cmd_param, env, status, curr_cmd.pos);
		ft_free_split((char **)envp);
		free_cmd_list(list);
		table_delete_table(env);
		*status = 0;
		exit(*status);
	}
	path = get_cmd_path(cmd_param, env);
	table_delete_table(env);
	if (path != NULL && access(path, X_OK) == 0)
		execve(path, cmd_param.cmd, (char **)envp);
	ft_free_split((char **)envp);
	free_exit_error_exec(list, status, curr_cmd.index, path);
}

pid_t	create_child_and_exec_cmd(t_cmd_list *list, t_hash_table *env,
			int *status, t_curr_cmd cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		write(2, "minishell: fork: cannot fork\n", 30);
		*status = 1;
	}
	else if (pid == 0)
		exec_cmd(list, env, cmd, status);
	return (pid);
}

void	create_childs_and_exec_cmds(t_cmd_list *list, t_hash_table *env,
			pid_t *pid, int *status)
{
	size_t		i;
	t_curr_cmd	cmd;

	i = 0;
	while (i < list->nb_cmd)
	{
		cmd.cmd = list->cmds[i];
		cmd.pos = get_pos(list, i);
		cmd.index = i;
		if (cmd.pos != LAST && cmd.pos != ALONE && pipe(cmd.fds) == -1)
			write(2, "minishell: pipe: cannot pipe\n", 30);
		if (list->cmds[i].io[0] != -1 && list->cmds[i].io[1] != -1)
			*pid = create_child_and_exec_cmd(list, env, status, cmd);
		if (cmd.pos != FIRST && cmd.pos != ALONE)
			close(cmd.fds[2]);
		if (cmd.pos != LAST && cmd.pos != ALONE)
		{
			close(cmd.fds[1]);
			cmd.fds[2] = cmd.fds[0];
		}
		i++;
	}
}

int	exec_cdms_list(t_cmd_list *list, t_hash_table *env,
		struct termios old_termios)
{
	const int	is_alone = get_pos(list, 0) == ALONE;
	pid_t		pid;
	int			status;
	int			exit_status;

	set_signal_handler_exec(old_termios);
	status = 0;
	exit_status = 0;
	if (is_alone && is_builtin(list->cmds[0]))
		exit_status = launch_builtin(list->cmds[0], env, &status, ALONE);
	else
		create_childs_and_exec_cmds(list, env, &pid, &status);
	if ((!is_alone || (is_alone && !is_builtin(list->cmds[0])))
		&& count_cmds_with_correct_io(list) >= 1)
	{
		waitpid(pid, &status, 0);
		while (wait(0) > 0)
			;
	}
	if (count_cmds_with_correct_io(list) >= 1)
		table_insert(env, ft_strdup("?"),
			ft_itoa(calc_correct_status(list, status, is_alone)));
	close_all_io(list);
	return (exit_status);
}
