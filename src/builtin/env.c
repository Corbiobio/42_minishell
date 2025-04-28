/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:31:30 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/28 15:41:01 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

static void	print_env(t_hash_table *env)
{
	size_t		i;
	t_ht_item	curr_item;

	i = 0;
	while (i < env->capacity)
	{
		curr_item = env->items[i];
		if (curr_item.key && ft_strcmp(curr_item.key, "?") == 0)
		{
			i++;	
			continue ;
		}
		if (curr_item.key && curr_item.value)
			printf("%s=%s\n", curr_item.key, curr_item.value);
		i++;
	}
}

void	ft_env(t_cmd cmd, t_hash_table *env, int *status)
{
	(void)cmd;
	print_env(env);
	*status = 0;
}