/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:26:59 by edarnand          #+#    #+#             */
/*   Updated: 2025/05/19 11:29:23 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char	*join_path_to_cmd(char *path, char *cmd_name)
{
	char	*new_path;
	char	*tmp;

	tmp = ft_strjoin(path, "/");
	new_path = ft_strjoin(tmp, cmd_name);
	free(tmp);
	return (new_path);
}

char	*get_absolut_path(char *cmd, const char **paths)
{
	if (ft_strchr(cmd, '/') != NULL)
	{
		ft_free_split((char **)paths);
		return (ft_strdup(cmd));
	}
	return (NULL);
}

char	*get_path(const char **paths, char *cmd)
{
	size_t	i;
	char	*tmp_path;
	char	*path;

	i = 0;
	path = NULL;
	while (paths[i] != NULL)
	{
		tmp_path = join_path_to_cmd((char *)paths[i], cmd);
		if (tmp_path == NULL)
			break ;
		if (access(tmp_path, F_OK) == 0)
		{
			free(path);
			path = tmp_path;
		}
		else
			free(tmp_path);
		i++;
	}
	return (path);
}

char	*get_cmd_path(t_cmd cmd, t_hash_table *env)
{
	const char	*paths_from_env = table_search(env, "PATH");
	const char	**paths = (const char**)ft_split(paths_from_env, ':');
	char		*path;

	path = get_absolut_path(cmd.cmd[0], paths);
	if (path != NULL)
		return (path);
	if (paths == NULL)
		return (NULL);
	path = get_path(paths, cmd.cmd[0]);
	ft_free_split((char **)paths);
	return (path);
}
