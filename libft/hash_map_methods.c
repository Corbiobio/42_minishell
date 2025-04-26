/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map_methods.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:21:23 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/26 12:11:39 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

int	power(int nb, int exponent)
{
	if (exponent < 0)
		return (0);
	if (exponent <= 1)
		return (nb);
	return (power(nb * nb, exponent - 1));
}

int	table_hash_function(const char *key, const int len_table)
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
		hash = hash % (len_table - 1);
		i++;
	}
	return ((int)hash);
}

#include <stdio.h>

void	print_hash_table_simpler(t_hash_table *table)
{
	size_t	i;

	printf("size: %zu, capacity: %zu\n", table->size, table->capacity);
	i = 0;
	while (i < table->capacity)
	{
		printf("key: %p, value: %p\n", table->items[i].key, table->items[i].value);
		i++;
	}
}

int	table_insert(t_hash_table *table, char *key, char *value)
{
	size_t		i;
	const int	load = table->size * 100 / table->capacity;

	if (load > 50 && table_resize(table) == 1)
			return (1);
	i = table_hash_function(key, table->capacity);
	while (i < table->capacity && table->items[i].key)
	{
		if (table->items[i].value && ft_strcmp(table->items[i].key, key) == 0)
		{
			table_delete_item(table, i);
			break ;
		}
		i++;
	}
	if (i < table->capacity)
	{
		table->items[i] = (t_ht_item){.key = key, .value = value};
		table->size++;
	}
	else
	{
		if (table_resize(table) == 1)
			return (1);
		table_insert(table, key, value);
	}
	return (0);
}

char	*table_search(t_hash_table *table, const char *key)
{
	size_t	i;

	i = table_hash_function(key, table->capacity);
	while (i < table->capacity && table->items[i].key)
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
	size_t		i;

	i = table_hash_function(key, table->capacity);
	while (table->capacity && table->items[i].key)
	{
		if (table->items[i].value && ft_strcmp(table->items[i].key, key) == 0)
		{
			table_delete_item(table, i);
			table->items[i].key = &deleted_item;
		}
		i++;
	}
	if (i < table->capacity)
		table->size--;
}
