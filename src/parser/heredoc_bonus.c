/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 08:53:58 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/27 11:34:43 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>

static void	signal_handler_heredoc(int signum)
{
	(void)signum;
}

static void	set_signal_handler_heredoc(void)
{
	struct sigaction	sigset;
	struct termios		termios;

	sigemptyset(&sigset.sa_mask);
	sigaddset(&sigset.sa_mask, SIGINT);
	sigset.sa_flags = 0;
	sigset.sa_handler = &signal_handler_heredoc;
	sigaction(SIGINT, &sigset, 0);
	tcgetattr(STDIN_FILENO, &termios);
	termios.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &termios);
}
static int	write_heredoc(char *eof_signal, int write_end)
{
	char	*buffer;
	int		len_eof;

	set_signal_handler_heredoc();
	len_eof = ft_strlen(eof_signal);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		buffer = get_next_line(STDIN_FILENO);
		if (buffer == 0)
		{
			write(STDOUT_FILENO, "\n", 1);
			return (-1);
		}
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
