/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:26:59 by edarnand          #+#    #+#             */
/*   Updated: 2025/05/02 12:33:05 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>
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

char	*get_cmd_path(t_cmd cmd, t_hash_table *env)
{
	const char	*paths_from_env = table_search(env, "PATH");
	const char	**paths = (const char**)ft_split(paths_from_env, ':');
	char		*curr_path;
	size_t		i;

	if (paths == NULL)
		return (NULL);
	if (ft_strchr(cmd.cmd[0], '/') != NULL)
	{
		ft_free_split((char **)paths);
		return (ft_strdup(cmd.cmd[0]));
	}
	i = 0;
	while (paths[i] != NULL)
	{
		curr_path = join_path_to_cmd((char *)paths[i], cmd.cmd[0]);
		if (curr_path == NULL || access(curr_path, X_OK) == 0)
			break ;
		free(curr_path);
		i++;
		if (paths[i] == NULL)
			curr_path = NULL;
	}
	ft_free_split((char **)paths);
	return (curr_path);
}
