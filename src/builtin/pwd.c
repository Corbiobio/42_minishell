/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:18:59 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/30 16:32:06 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdio.h>

void	ft_pwd(int *status)
{
	char	*pwd;
	char	*tmp_cwd;

	tmp_cwd = malloc(PATH_MAX + 1);
	if (tmp_cwd == 0)
		return ;
	if (getcwd(tmp_cwd, PATH_MAX + 1) == 0)
		return (free(tmp_cwd));
	pwd = ft_strdup(tmp_cwd);
	if (pwd == 0)
		return (free(tmp_cwd));
	if (write(STDIN_FILENO, pwd, ft_strlen(pwd)) == -1)
	{
		perror("minishell: pwd:");
		*status = 1;
		return ;
	}
	if (write(STDIN_FILENO, "\n", 1) == -1)
	{
		perror("minishell: pwd:");
		*status = 1;
		return ;
	}
	*status = 0;
	return ;
}
