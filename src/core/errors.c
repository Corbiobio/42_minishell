/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:57:38 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/30 18:58:25 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <unistd.h>

int	print_error_set_status(t_error error, t_hash_table *env)
{
	write(2, "minishell: ", 11);
	if (error == ERROR_QUOTE_UNCLOSED)
	{
		table_insert(env, ft_strdup("?"), ft_strdup("2"));
		write(2, "unclosed quote\n", 15);
	}
	else if (error == ERROR_FILENAME)
	{
		table_insert(env, ft_strdup("?"), ft_strdup("2"));
		write(2, "missing filename after redirection\n", 35);
	}
	return (1);
}

int	print_error_dont_set_status(t_error error)
{
	write(2, "minishell: ", 11);
	if (error == ERROR_HEREDOC_EOF)
		write(2, "warning: heredoc delimited by end of file\n", 42);
	return (1);
}

int	perror_set_status(t_hash_table *env, int status, char *optional)
{
	write(2, "minishell: ", 11);
	table_insert(env, ft_strdup("?"), ft_itoa(status));
	perror(optional);
	return (1);
}

int	set_status(t_hash_table *env, int status)
{
	table_insert(env, ft_strdup("?"), ft_itoa(status));
	return (1);
}
