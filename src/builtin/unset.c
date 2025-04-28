/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:10:53 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/28 18:11:46 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>

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
		printf("arg %s -> %s\n", args[i], table_search(env,args[i]));
		table_remove_item(env, (char *)args[i]);
		i++;
	}
}
