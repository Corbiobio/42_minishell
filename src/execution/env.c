/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:14:51 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/02 11:47:46 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

char	**env_init(t_hash_table *env_table)
{
	size_t		nb_var;
	size_t		i;
	char		**env;

	i = 0;
	nb_var = 1;
	while (i < env_table->capacity)
	{
		if (env_table->items[i].value)
			nb_var++;
		i++;
	}
	env = malloc(sizeof(char *) * nb_var);
	return (env);
}

char	**get_env_from_table(t_hash_table *env_table)
{
	size_t		i;
	size_t		j;
	char		**env;
	t_ht_item	item;

	env = env_init(env_table);
	if (env == 0)
		return (0);
	i = 0;
	j = 0;
	while (i < env_table->capacity)
	{
		item = env_table->items[i];
		if (item.value)
		{
			env[j] = ft_str_triple_join(item.key, "=", item.value);
			if (env[j] == 0)
				return (ft_free_split(env));
			j++;
		}
		i++;
	}
	env[j] = 0;
	return (env);
}
