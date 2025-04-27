/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:13:00 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/25 17:05:31 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static int	is_flag(char *str)
{
	if (*str == '-')
		str++;
	if (*str == '\0')
		return (0);
	while (*str == 'n')
		str++;
	if (*str == '\0')
		return (1);
	return (0);
}

void	ft_echo(t_cmd cmd, int *status)
{
	const char	**arg = (const char **)cmd.cmd + 1;
	int			i;
	int			flag;

	i = 0;
	flag = 0;
	while (arg[i] != NULL && is_flag((char *)arg[i]))
	{
		flag = 1;
		i++;
	}
	while (arg[i] != NULL)
	{
		write(STDOUT_FILENO, arg[i], ft_strlen(arg[i]));
		if (arg[i + 1] != NULL)
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (!flag)
		write(STDOUT_FILENO, "\n", 1);
	*status = EXIT_SUCCESS;
}
