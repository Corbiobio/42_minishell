/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:57:38 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/09 15:12:55 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int	print_error_set_status(t_error error, t_hash_table *env)
{
	write(STDERR_FILENO, "minishell: ", 11);
	if (error == ERROR_QUOTE_UNCLOSED)
	{
		table_insert(env, ft_strdup("?"), ft_strdup("2"));
		write(STDERR_FILENO, "unclosed quote\n", 15);
	}
	else if (error == ERROR_FILENAME)
	{
		table_insert(env, ft_strdup("?"), ft_strdup("2"));
		write(STDERR_FILENO, "missing filename after redirection\n", 35);
	}
	else if (error == ERROR_NO_CMD)
	{
		table_insert(env, ft_strdup("?"), ft_strdup("2"));
		write(STDERR_FILENO, "missing command\n", 16);
	}
	return (1);
}

int	print_error_dont_set_status(t_error error)
{
	write(STDERR_FILENO, "minishell: ", 11);
	if (error == ERROR_HEREDOC_EOF)
		write(STDERR_FILENO, "warning: heredoc delimited by end of file\n", 42);
	return (1);
}

int	perror_set_status(t_hash_table *env, int status, char *optional)
{
	write(STDERR_FILENO, "minishell: ", 11);
	table_insert(env, ft_strdup("?"), ft_itoa(status));
	perror(optional);
	return (1);
}

int	set_status(t_hash_table *env, int status)
{
	table_insert(env, ft_strdup("?"), ft_itoa(status));
	return (1);
}
