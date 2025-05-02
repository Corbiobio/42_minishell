/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:18:59 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/02 12:37:00 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

void	ft_pwd(int *status)
{
	char	*pwd;
	char	*tmp_cwd;

	tmp_cwd = malloc(PATH_MAX + 1);
	if (tmp_cwd == 0)
	{
		*status = 1;
		return ;
	}
	if (getcwd(tmp_cwd, PATH_MAX + 1) == 0)
	{
		*status = 1;
		return (free(tmp_cwd));
	}
	pwd = ft_strdup(tmp_cwd);
	if (pwd == 0)
	{
		*status = 1;
		return (free(tmp_cwd));
	}
	write(STDOUT_FILENO, pwd, ft_strlen(pwd));
	write(STDOUT_FILENO, "\n", 1);
	free(tmp_cwd);
	free(pwd);
	*status = 0;
}
