/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:04:17 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/23 16:51:22 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/execution.h"
#include "../../includes/parser.h"
#include "../../libft/libft.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/wait.h>

void	print_hash_table(t_hash_table *table)
{
	size_t	i;

	printf("size: %zu, capacity: %zu\n", table->size, table->capacity);
	i = 0;
	while (i < table->capacity)
	{
		if (table->items[i].key == 0)
			printf("%zu: no value  ", i);
		else if (table->items[i].value == 0)
			printf("%zu: empty/removed  ", i);
		else
			printf("%zu: ,key: %s, value: %s\n", i, table->items[i].key, table->items[i].value);
		i++;
	}
}

int	split_key_value(char *input, char **key, char **value)
{
	size_t	i;

	i = 0;
	while (input[i] && input[i] != '=')
		i++;
	if (!input[i])
		return (1);
	*key = ft_strndup(input, i);
	*value = ft_strdup(&input[i + 1]);
	if (*key == 0 || *value == 0)
	{
		free(*key);
		free(*value);
		return (1);
	}
	return (0);
}

t_hash_table	*convert_env_to_table(char **env)
{
	t_hash_table	*env_table;
	char			*key;
	char			*value;
	size_t			i;

	env_table = table_alloc(100);
	if (env_table == 0)
		return (0);
	i = 0;
	while (env[i])
	{
		if (split_key_value(env[i], &key, &value) == 1)
		{
			table_delete_table(env_table);
			return (0);
		}
		table_insert(env_table, key, value);
		i++;
	}
	return (env_table);
}

int	main(int ac, char **av, char **env)
{
	int				status;
	t_cmd_list		*list;
	t_hash_table	*env_table;

	env_table = convert_env_to_table(env);
	if (env_table == 0)
		return (1);
	print_hash_table(env_table);
	list = parser("ls < ' cat |'file1 >file2|cat $USER > ooomfie", env_table);
	create_child_and_exec_cmd(list, env);
	while (wait(&status) > 0)
		;
	free(list);
	(void)ac;
	(void)av;
	return (status);
}
