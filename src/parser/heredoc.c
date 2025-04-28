/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 08:53:58 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/28 09:59:21 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"
#include "parser.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>

static void	set_signal_handler_heredoc(void)
{
	struct sigaction	sigset;
	struct termios		termios;

	sigemptyset(&sigset.sa_mask);
	sigaddset(&sigset.sa_mask, SIGINT);
	sigset.sa_flags = 0;
	sigset.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sigset, 0);
	tcgetattr(STDIN_FILENO, &termios);
	termios.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &termios);
}

void	delete_all_heredoc(t_free_close *stuff)
{
	close(stuff->fd_read_end);
	free(stuff);
}

static int	heredoc_child(char *eof, int write_end, t_free_close *stuff)
{
	char	*buffer;
	int		len_eof;

	delete_all_heredoc(stuff);
	set_signal_handler_heredoc();
	len_eof = ft_strlen(eof);
	while (1)
	{
		buffer = readline("> ");
		if (buffer == 0)
		{
			print_error_return_one(ERROR_HEREDOC_EOF);
			break ;
		}
		if (ft_strncmp(buffer, eof, len_eof + 1) == 0)
			break ;
		if (write(write_end, buffer, ft_strlen(buffer)) == -1)
		{
			close(write_end);
			free(buffer);
			exit(EXIT_FAILURE);
		}
		free(buffer);
	}
	close(write_end);
	free(buffer);
	exit(EXIT_FAILURE);
}

static int	write_heredoc(char *eof, int write_end, t_free_close *stuff)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid < 0)
		return (1);
	else if (pid == 0)
		heredoc_child(eof, write_end, stuff);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (1);
	}
	return (1);
}

int	create_heredoc(char *eof, t_free_close *stuff)
{
	int		end[2];
	char	*eof_signal;

	if (pipe(end) == -1)
		return (-1);
	if (write_heredoc(eof, end[1], stuff) == 1)
	{
		close(end[1]);
		close(end[0]);
		return (-1);
	}
	close(end[1]);
	return (end[0]);
}
