/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:04:03 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/09 15:14:37 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	ft_exit(t_cmd cmd, int *status, t_position pos, int stds[2])
{
	int	arg;

	arg = 0;
	set_stds_to_default(cmd, pos, stds);
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
	*status = arg;
	if (pos == ALONE)
		return (42);
	return (0);
}
