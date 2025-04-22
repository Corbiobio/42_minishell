/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 08:53:58 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/22 10:05:13 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include <unistd.h>

static int	write_heredoc(char	*eof_signal, int write_end)
{
	char	*buffer;
	int		len_eof;

	len_eof = ft_strlen(eof_signal);
	while (1)
	{
		buffer = get_next_line(STDIN_FILENO);
		if (buffer == 0)
			return (-1);
		if (ft_strncmp(buffer, eof_signal, len_eof + 1) == 0)
			break ;
		if (write(write_end, buffer, ft_strlen(buffer)) == -1)
		{
			free(buffer);
			return (-1);
		}
		free(buffer);
	}
	free(buffer);
	return (0);
}

int	create_heredoc(char *eof)
{
	int		end[2];
	char	*eof_signal;

	if (pipe(end) == -1)
		return (-1);
	eof_signal = ft_strjoin(eof, "\n");
	if (eof_signal == 0)
	{
		close(end[0]);
		close(end[1]);
		return (-1);
	}
	if (write_heredoc(eof_signal, end[1]) == -1)
	{
		close(end[1]);
		close(end[0]);
		free(eof_signal);
		return (-1);
	}
	free(eof_signal);
	close(end[1]);
	return (end[0]);
}
