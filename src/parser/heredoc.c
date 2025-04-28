/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 08:53:58 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/28 12:02:46 by sflechel         ###   ########.fr       */
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

static int	g_signum = 0;

static void	signal_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		close(STDIN_FILENO);
		g_signum = SIGINT;
	}
}

static void	set_signal_handler_heredoc(void)
{
	struct sigaction	sigset;
	struct termios		termios;

	sigemptyset(&sigset.sa_mask);
	sigaddset(&sigset.sa_mask, SIGINT);
	sigset.sa_flags = SA_RESTART;
	sigset.sa_handler = &signal_handler_heredoc;
	sigaction(SIGINT, &sigset, 0);
	tcgetattr(STDIN_FILENO, &termios);
	termios.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &termios);
}

void	delete_all_heredoc(t_free_close *stuff)
{
	close(stuff->fd_read_end);
	free(stuff->line1);
	free(stuff->line2);
	free(stuff->line3);
	free(stuff->cmds);
	table_delete_table(stuff->env);
	free(stuff);
}

void	write_protected_heredoc(char *line, int write_end, t_hash_table *env)
{
	t_tokenized_line	*tokens;

	tokens = expander(line, env);
	if (tokens == 0)
	{
		close(write_end);
		free(line);
		exit(EXIT_FAILURE);
	}
	if (write(write_end, tokens->line, ft_strlen(tokens->line)) == -1)
	{
		free(tokens->line);
		free(tokens);
		close(write_end);
		exit(EXIT_FAILURE);
	}
	if (write(write_end, "\n", 1) == -1)
	{
		free(tokens->line);
		free(tokens);
		close(write_end);
		exit(EXIT_FAILURE);
	}
	printf("%i, %s\n", write_end, tokens->line);
	free(tokens->line);
	free(tokens);
}

static int	heredoc_child(char *eof, int write_end, t_free_close *stuff)
{
	char	*line;
	int		len_eof;

	set_signal_handler_heredoc();
	len_eof = ft_strlen(eof);
	while (1)
	{
		line = readline("> ");
		if (line == 0)
		{
			if (g_signum == SIGINT)
			{
				g_signum = 0;
				close(write_end);
				free(eof);
				delete_all_heredoc(stuff);
				exit(1);
			}
			else
				print_error_return_one(ERROR_HEREDOC_EOF);
			break ;
		}
		if (ft_strncmp(line, eof, len_eof + 1) == 0)
			break ;
		write_protected_heredoc(line, write_end, stuff->env);
	}
	delete_all_heredoc(stuff);
	close(write_end);
	free(eof);
	exit(EXIT_SUCCESS);
}

static int	write_heredoc(char *eof, int write_end, t_free_close *stuff)
{
	int	pid;
	int	status;

	status = 0;
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

	if (pipe(end) == -1)
		return (-1);
	stuff->fd_read_end = end[0];
	if (write_heredoc(eof, end[1], stuff) == 1)
	{
		close(end[1]);
		close(end[0]);
		return (-1);
	}
	close(end[1]);
	return (end[0]);
}
