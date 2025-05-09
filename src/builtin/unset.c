/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:10:53 by edarnand          #+#    #+#             */
/*   Updated: 2025/05/09 15:15:01 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_unset(t_cmd cmd, t_hash_table *env, int *status)
{
	const char	**args = (const char **)cmd.cmd + 1;
	int			i;

	*status = 0;
	if (args[0] == NULL)
		return ;
	i = 0;
	while (args[i] != NULL)
	{
		table_remove_item(env, (char *)args[i], MODE_REMOVE, i);
		i++;
	}
}
