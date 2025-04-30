/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:57:53 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/30 16:56:56 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

static int	count_args(const char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		i++;
	return (i);
}

static char	*get_start_path(char *path)
{
	char	*start_path;

	if (path[0] == '/')
		start_path = ft_strdup("/");
	else
	{
		start_path = malloc(PATH_MAX * sizeof(char));
		if (getcwd(start_path, PATH_MAX) == NULL)
			write(2, "minishell: cd: cannot use your current directory\n", 50);
	}
	return (start_path);
}

static char *add_dirs_to_path(char *start_path, char **dirs)
{
	int		curr_len;
	char	*final_path;
	char	*tmp_path;
	int		i;

	final_path = start_path;
	i = 0;
	while (dirs[i] != NULL)
	{
		curr_len = ft_strlen(final_path);
		if (curr_len >= 2 && ft_strcmp(dirs[i], "..") == 0)
		{
			if (final_path[curr_len - 1] == '/')
				final_path[curr_len - 1] = '\0';
			final_path[strchr_last_index(final_path, '/') + 1] = '\0';
		}
		else if (ft_strcmp(dirs[i], ".") != 0)
		{
			
			if (final_path[curr_len - 1] != '/')
			{
				tmp_path = ft_strjoin(final_path, "/");
				free(final_path);
				final_path = tmp_path;
			}
			tmp_path = ft_strjoin(final_path, dirs[i]);
			free(final_path);
			final_path = tmp_path;
		}
		i++;
	}
	return (final_path);
}
static void	handle_path(char *path, int *status)
{
	char	*start_path;
	char	*final_path;
	char	**dirs;
	
	if (path == NULL || path[0] == '\0')
		return ;
	start_path = get_start_path(path);
	dirs = ft_split(path, '/');
	if (start_path == NULL || dirs == NULL)
	{
		free(start_path);
		ft_free_split(dirs);
		*status = 1;
		return ;
	}
	printf("path to add --%s--\n", path);
	printf("start_path __%s__\n", start_path);
	printf("-----------------------\n");
	final_path = add_dirs_to_path(start_path, dirs);
	printf("-----------------------\n");
	printf("final_path __%s__\n", final_path);
	if (final_path == NULL)
		*status = 1;
	else
	{
		if (ft_strlen(final_path) >= PATH_MAX)
		{
			write(2, "minishell: cd: file name too long\n", 25);
			*status = 1;
		}
		else
			chdir(final_path);
	}
	free(final_path);
	ft_free_split(dirs);
}

static void	go_home(t_hash_table *env, int *status)
{
	char *path;

	path = table_search(env, "HOME");
	if (path == NULL)
	{
		write(2, "minishell: cd: HOME not set\n", 29);
		*status = 1; 
	}
	else
		handle_path(path, status);
}

void	ft_cd(t_cmd cmd, t_hash_table *env, int *status)
{
	const char	**args = (const char **)cmd.cmd + 1;
	const int	args_count = count_args(args);

	*status = 0;
	if (args_count >= 2)
	{
		write(2, "minishell: cd: too many arguments\n", 35);
		*status = 1;
	}
	else if (args_count == 0)
		go_home(env, status);
	else
		handle_path((char *)args[0], status);
}
