/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:26:59 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/23 12:46:01 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <unistd.h>

static char	*find_path_variable_in_env(char **env)
{
	int		i;
	char	*paths;

	if (env == NULL)
		return (NULL);
	i = 0;
	paths = NULL;
	while (env[i] != NULL)
	{
		if (ft_strnstr(env[i], "PATH=", 5) != NULL)
		{
			paths = env[i] + 5;
			break ;
		}
		i++;
	}
	return (paths);
}

static char	*join_path_to_cmd(char *path, char *cmd_name)
{
	char	*new_path;
	char	*tmp;

	tmp = ft_strjoin(path, "/");
	new_path = ft_strjoin(tmp, cmd_name);
	free(tmp);
	return (new_path);
}

char	*get_cmd_path(t_cmd cmd, char **env)
{
	const char	*paths_var = find_path_variable_in_env(env);
	const char	**paths = (const char**)ft_split(paths_var, ':');
	char		*curr_path;
	size_t		i;

	if (paths == NULL)
		return (NULL);
	if (ft_strchr(cmd.cmd[0], '/') != NULL)
		return (ft_strdup(cmd.cmd[0]));
	i = 0;
	while (paths[i] != NULL)
	{
		curr_path = join_path_to_cmd((char *)paths[i], cmd.cmd[0]);
		if (curr_path == NULL || access(curr_path, X_OK) == 0)
			break ;
		free(curr_path);
		i++;
	}
	ft_free_split((char **)paths);
	return (curr_path);
}
