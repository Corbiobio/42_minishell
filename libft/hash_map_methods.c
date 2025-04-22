/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map_methods.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:21:23 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/22 15:24:22 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	power(int nb, int exponent)
{
	if (exponent < 0)
		return (0);
	if (exponent <= 1)
		return (nb);
	return (power(nb * nb, exponent - 1));
}

static int	table_hash_function(char *key, int len_table)
{
	long long int	hash;
	int				prime_signature;
	int				len_key;
	int				i;

	prime_signature = 163;
	len_key = ft_strlen(key);
	hash = 0;
	i = 0;
	while (i < len_key)
	{
		hash += power(prime_signature, len_key - i + 1) * key[i];
		hash = hash % len_table;
		i++;
	}
	return ((int)hash);
}

int	table_insert(t_hash_table *table, char *key, char *value)
{
	int			i;
	const int	load = table->capacity * 100 / table->size;

	if (load > 50)
	{
		if (table_resize(table) == 1)
			return (1);
	}
	i = table_hash_function(key, table->capacity);
	while (table->items[i].key)
	{
		if (table->items[i].value && ft_strcmp(table->items[i].key, key) == 0)
		{
			table_delete_item(table, i);
			break ;
		}
		i++;
	}
	table->items[i] = (t_ht_item){.key = key, .value = value};
	table->size++;
	return (0);
}

char	*table_search(t_hash_table *table, char *key)
{
	int	i;

	i = table_hash_function(key, table->capacity);
	while (table->items[i].key)
	{
		if (table->items[i].value && ft_strcmp(table->items[i].key, key) == 0)
			return (table->items[i].value);
		i++;
	}
	return (0);
}

void	table_remove_item(t_hash_table *table, char *key)
{
	static char	deleted_item;
	int			i;

	i = table_hash_function(key, table->capacity);
	while (table->items[i].key)
	{
		if (table->items[i].value && ft_strcmp(table->items[i].key, key) == 0)
		{
			table_delete_item(table, i);
			table->items[i].key = &deleted_item;
		}
		i++;
	}
	table->size--;
}
