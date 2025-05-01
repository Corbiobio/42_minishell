/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:57:53 by edarnand          #+#    #+#             */
/*   Updated: 2025/05/01 18:36:32 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

void	get_cwd(char **cwd)
{
	*cwd = ft_calloc(sizeof(char), PATH_MAX);
	if (getcwd(*cwd, PATH_MAX) == NULL)
		write(2, "minishell: cd: cannot use your current directory\n", 50);
}

int	move_to_final_path(t_hash_table *env, char *path_to_add, char *cwd)
{
	struct stat sb;

	if (chdir(path_to_add) == -1)
	{
		write(2, "minishell: cd: ", 16);
		write(2, path_to_add, ft_strlen(path_to_add));
		if (ft_strlen(path_to_add) >= PATH_MAX)
			write(2, " file name too long\n", 21);
		else if (stat(path_to_add, &sb) == -1)
			write(2, " no such file or directory\n", 28);
		else if (S_ISREG(sb.st_mode))
			write(2, " not a directory\n", 18);
		else
			write(2, " permission denied\n", 20);
		return (free_1_return_1( cwd));
	}
	table_insert(env, ft_strdup("OLD_PWD"), cwd);
	get_cwd(&cwd);
	if (cwd == NULL || cwd[0] == '\0')
		return (free_1_return_1(cwd));
	table_insert(env, ft_strdup("PWD"), cwd);
	return (0);
}

void	handle_path(t_hash_table *env, char *path_to_add, int *status)
{
	char	*cwd;
	
	if (path_to_add == NULL || path_to_add[0] == '\0')
		return ;
	cwd = NULL;
	get_cwd(&cwd);
	if (cwd == NULL)
	{
		*status = 1;
		return (free_1(cwd));
	}
	*status = move_to_final_path(env, path_to_add, cwd);
}

void	go_home(t_hash_table *env, int *status)
{
	char *path;

	path = table_search(env, "HOME");
	if (path == NULL)
	{
		write(2, "minishell: cd: HOME not set\n", 29);
		*status = 1; 
	}
	handle_path(env, path, status);
}

void	ft_cd(t_cmd cmd, t_hash_table *env, int *status)
{
	const char	**args = (const char **)cmd.cmd + 1;

	*status = 0;
	if (cmd.nb_arg >= 2)
	{
		write(2, "minishell: cd: too many arguments\n", 35);
		*status = 1;
	}
	else if (cmd.nb_arg == 0)
		go_home(env, status);
	else
		handle_path(env, (char *)args[0], status);
}
