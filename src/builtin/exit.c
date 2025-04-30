/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:04:03 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/30 11:09:38 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

int	ft_exit(t_cmd cmd, int *status, t_position pos)
{
	int	arg;

	arg = 0;
	if (pos == ALONE)
		write(STDIN_FILENO, "exit\n", 5);
	if (cmd.nb_arg > 1)
	{
		write(STDERR_FILENO, "minishell: exit: too many arguments\n", 36);
		*status = 1;
		return (0);
	}
	if (cmd.nb_arg == 1 && safe_atoi(cmd.cmd[1], &arg) == 1)
	{
		write(STDERR_FILENO,
			"minishell: exit: numeric argument required\n", 43);
		*status = 2;
		return (0);
	}
	if (cmd.nb_arg == 1)
		*status = arg;
	else if (pos == ALONE)
		*status = 0;
	if (pos == ALONE)
		return (42);
	return (0);
}
