/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:57:38 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/27 12:44:24 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	print_error_return_one(t_error error)
{
	write(2, "minishell: ", 11);
	if (error == ERROR_QUOTE_UNCLOSED)
		write(2, "unclosed quote\n", 15);
	else if (error == ERROR_REDIRECTION_NO_FILENAME)
		write(2, "missing filename after redirection\n", 35);
	return (1);
}
