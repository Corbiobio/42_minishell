/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:06:46 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/28 17:37:53 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "builtin.h"
#include "libft.h"
#include <stdio.h>

int	launch_builtin(t_cmd cmd, t_hash_table *env, int *status)
{
	int	is_builtin;

	is_builtin = 0;
	if (ft_strcmp(cmd.cmd[0], "echo") == 0)
	{
		is_builtin = 1;
		ft_echo(cmd, status);
	}
	else if (ft_strcmp(cmd.cmd[0], "export") == 0)
	{
		is_builtin = 1;
		ft_export(cmd, env, status);
	}
	else if (ft_strcmp(cmd.cmd[0], "env") == 0)
	{
		is_builtin = 1;
		printf("env is in developpement\n");
	}
	else if (ft_strcmp(cmd.cmd[0], "cd") == 0)
	{
		is_builtin = 1;
		printf("cd is in developpement\n");
	}
	else if (ft_strcmp(cmd.cmd[0], "pwd") == 0)
	{
		is_builtin = 1;
		printf("pwd is in developpement\n");
	}
	else if (ft_strcmp(cmd.cmd[0], "unset") == 0)
	{
		is_builtin = 1;
		ft_unset(cmd, env, status);
	}
	else if (ft_strcmp(cmd.cmd[0], "exit") == 0)
	{
		is_builtin = 1;
		printf("exit is in developpement\n");
	}
	return (is_builtin);
}

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
