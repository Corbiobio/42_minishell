/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:06:46 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/30 17:11:51 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "builtin.h"
#include "libft.h"

int	is_builtin(t_cmd cmd)
{
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

int	launch_builtin(t_cmd cmd, t_hash_table *env, int *status, t_position pos)
{
	const int	am_builtin = is_builtin(cmd);

	if (cmd.io[0] == -1 || cmd.io[1] == -1)
		return (am_builtin);
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
		if (ft_exit(cmd, status, pos) == 42)
			return (42);
	}
	return (am_builtin);
}
