/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:04:03 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/29 18:24:04 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <unistd.h>

void	ft_exit(int *status, t_position pos)
{
	if (pos == ALONE)
	{
		write(STDIN_FILENO, "exit\n", 5);
		*status = 42;
	}
	else
		*status = 0;
}
