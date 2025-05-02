/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:13:02 by edarnand          #+#    #+#             */
/*   Updated: 2025/05/02 17:52:26 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void	print_export(t_hash_table *env)
{
	size_t		i;
	t_ht_item	curr_item;

	i = 0;
	while (i < env->capacity)
	{
		curr_item = env->items[i];
		if (curr_item.key && (ft_strcmp(curr_item.key, "?") == 0
				|| ft_strcmp(curr_item.key, "_") == 0))
		{
			i++;
			continue ;
		}
		if (curr_item.key && curr_item.key[0] != '\0' && curr_item.value)
			printf("declare -x %s=\"%s\"\n", curr_item.key, curr_item.value);
		else if (curr_item.key && curr_item.key[0] != '\0')
			printf("declare -x %s\n", curr_item.key);
		i++;
	}
}

void	print_error(char *str, int *error)
{
	write(2, "minishell: export: ", 20);
	write(2, str, ft_strlen(str));
	write(2, " is not a valid identifier\n", 28);
	*error = 1;
}

static int	is_valid_str(char *str)
{
	size_t	i;

	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while ((ft_isalnum(str[i]) || str[i] == '_')
		&& str[i] != '=' && str[i] != '\0')
		i++;
	if (str[i] == '\0' || str[i] == '=' || ft_isalnum(str[i]))
		return (1);
	return (0);
}

static void	put_to_env(char *str, t_hash_table *env, int *error)
{
	char	*value;
	int		curr_error;

	curr_error = 0;
	value = ft_strchr(str, '=');
	if (value != NULL)
	{
		*value = '\0';
		value += 1;
		value = ft_strdup(value);
		if (value == NULL)
			curr_error = 1;
	}
	str = ft_strdup(str);
	if (str == NULL)
		curr_error = 1;
	if (curr_error == 0 && table_insert(env, str, value) == 1)
		curr_error = 1;
	if (curr_error == 1)
	{
		free(str);
		free(value);
		*error = 1;
	}
}

void	ft_export(t_cmd cmd, t_hash_table *env, int *status)
{
	const char	**args = (const char **)cmd.cmd + 1;
	size_t		i;
	int			error;

	i = 0;
	error = 0;
	if (args[0] == NULL)
		print_export(env);
	while (args[i] != NULL)
	{
		if (is_valid_str((char *)args[i]))
			put_to_env((char *)args[i], env, &error);
		else
			print_error((char *)args[i], &error);
		i++;
	}
	if (error)
		*status = EXIT_FAILURE;
	else
		*status = EXIT_SUCCESS;
}
