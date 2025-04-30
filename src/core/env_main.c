/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:39:13 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/30 13:32:47 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/limits.h>

int	insert_env_and_alloc(char *key_stat, char *value_stat, t_hash_table *env)
{
	char	*key;
	char	*value;

	key = ft_strdup(key_stat);
	value = ft_strdup(value_stat);
	if (key == 0 || value == 0)
		return (free_2_return_1(key, value));
	if (table_insert(env, key, value) == 1)
		return (free_2_return_1(key, value));
	return (0);
}

int	fill_empty_env(t_hash_table *env)
{
	char	*tmp_cwd;
	char	*key_oldpwd;

	if (insert_env_and_alloc("SHLVL", "0", env) == 1)
		return (1);
	if (insert_env_and_alloc("_", "usr/bin/env", env) == 1)
		return (1);
	key_oldpwd = ft_strdup("OLDPWD");
	if (key_oldpwd == 0)
		return (free_1_return_1(key_oldpwd));
	if (table_insert(env, key_oldpwd, 0) == 1)
		return (free_1_return_1(key_oldpwd));
	tmp_cwd = malloc(PATH_MAX + 1);
	if (tmp_cwd == 0)
		return (1);
	if (getcwd(tmp_cwd, PATH_MAX + 1) == 0)
		return (free_1_return_1(tmp_cwd));
	if (insert_env_and_alloc("PWD", tmp_cwd, env) == 1)
		return (free_1_return_1(tmp_cwd));
	free(tmp_cwd);
	return (0);
}

int	initiate_env(t_hash_table *env)
{
	char	*key;
	char	*value;

	if (insert_env_and_alloc("?", "0", env) == 1)
		return (1);
	key = ft_strdup("SHLVL");
	value = ft_itoa(ft_atoi(table_search(env, key)) + 1);
	if (key == 0 || value == 0)
		return (free_2_return_1(key, value));
	if (table_insert(env, key, value) == 1)
		return (free_2_return_1(key, value));
	return (0);
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
		return (free_2_return_1(*key, *value));
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
	if (*env == 0 && fill_empty_env(env_table) == 1)
		return (table_delete_return_null(env_table));
	i = 0;
	while (env[i])
	{
		if (split_key_value(env[i], &key, &value) == 1)
		{
			table_delete_table(env_table);
			return (0);
		}
		if (table_insert(env_table, key, value) == 1)
			return (table_delete_return_null(env_table));
		i++;
	}
	if (initiate_env(env_table) == 1)
		return (table_delete_return_null(env_table));
	return (env_table);
}
