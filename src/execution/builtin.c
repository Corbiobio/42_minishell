/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:06:46 by edarnand          #+#    #+#             */
/*   Updated: 2025/05/09 15:11:55 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	is_builtin(t_cmd cmd)
{
	if (cmd.cmd[0] == NULL)
		return (0);
	if (ft_strcmp(cmd.cmd[0], "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd.cmd[0], "export") == 0)
		return (1);
	else if (ft_strcmp(cmd.cmd[0], "env") == 0)
		return (1);
	else if (ft_strcmp(cmd.cmd[0], "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd.cmd[0], "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd.cmd[0], "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd.cmd[0], "exit") == 0)
		return (1);
	return (0);
}

void	redirect_std(t_cmd cmd, t_position pos, int stds[2])
{
	if (pos != ALONE)
		return ;
	if (cmd.io[0] >= 0)
	{
		stds[0] = dup(STDIN_FILENO);
		dup2(cmd.io[0], STDIN_FILENO);
	}
	if (cmd.io[1] >= 0)
	{
		stds[1] = dup(STDOUT_FILENO);
		dup2(cmd.io[1], STDOUT_FILENO);
	}
}

void	set_stds_to_default(t_cmd cmd, t_position pos, int stds[2])
{
	if (pos != ALONE)
		return ;
	if (cmd.io[0] >= 0)
	{
		dup2(stds[0], STDIN_FILENO);
		if (stds[0] >= 0)
			close(stds[0]);
	}
	if (cmd.io[1] >= 0)
	{
		dup2(stds[1], STDOUT_FILENO);
		if (stds[1] >= 0)
			close(stds[1]);
	}
}

int	launch_builtin(t_cmd cmd, t_hash_table *env, int *status, t_position pos)
{
	const int	am_builtin = is_builtin(cmd);
	int			stds[2];

	if (cmd.io[0] == -1 || cmd.io[1] == -1)
		return (am_builtin);
	redirect_std(cmd, pos, stds);
	if (ft_strcmp(cmd.cmd[0], "echo") == 0)
		ft_echo(cmd, status);
	else if (ft_strcmp(cmd.cmd[0], "export") == 0)
		ft_export(cmd, env, status);
	else if (ft_strcmp(cmd.cmd[0], "env") == 0)
		ft_env(cmd, env, status);
	else if (ft_strcmp(cmd.cmd[0], "cd") == 0)
		ft_cd(cmd, env, status);
	else if (ft_strcmp(cmd.cmd[0], "pwd") == 0)
		ft_pwd(status);
	else if (ft_strcmp(cmd.cmd[0], "unset") == 0)
		ft_unset(cmd, env, status);
	else if (ft_strcmp(cmd.cmd[0], "exit") == 0)
	{
		if (ft_exit(cmd, status, pos, stds) == 42)
			return (42);
	}
	set_stds_to_default(cmd, pos, stds);
	return (am_builtin);
}
