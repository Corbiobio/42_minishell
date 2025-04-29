/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:18:59 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/28 18:11:50 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdio.h>

void	ft_pwd(t_hash_table *env, int *status)
{
	char	*pwd;

	pwd = table_search(env, "PWD");
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
