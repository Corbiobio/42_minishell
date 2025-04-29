/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 08:53:58 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/29 11:21:45 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"
#include "parser.h"
#include <signal.h>
#include <stdio.h>
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

	sigemptyset(&sigset.sa_mask);
	sigaddset(&sigset.sa_mask, SIGINT);
	sigset.sa_flags = SA_RESTART;
	sigset.sa_handler = &signal_handler_heredoc;
	sigaction(SIGINT, &sigset, 0);
}

static void	signal_handler_parent(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "^C", 2);
		write(STDOUT_FILENO, "\n", 1);
	}
}

static void	set_signal_handler_parent(void)
{
	struct sigaction	sigset;

	sigemptyset(&sigset.sa_mask);
	sigaddset(&sigset.sa_mask, SIGINT);
	sigset.sa_flags = SA_RESTART;
	sigset.sa_handler = &signal_handler_parent;
	sigaction(SIGINT, &sigset, 0);
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

void	write_no_expand_heredoc(char *line, int write_end, t_free_close *stuff, char *eof)
{
	if (write(write_end, line, ft_strlen(line)) == -1)
	{
		free_2(line, eof);
		delete_all_heredoc(stuff);
		close(write_end);
		exit(EXIT_FAILURE);
	}
	if (write(write_end, "\n", 1) == -1)
	{
		free_2(line, eof);
		delete_all_heredoc(stuff);
		close(write_end);
		exit(EXIT_FAILURE);
	}
	free(line);
}

void	write_expander_heredoc(char *line, int write_end, t_free_close *stuff, char *eof)
{
	t_tokenized_line	*tokens;

	tokens = expander(line, stuff->env);
	if (tokens == 0)
	{
		close(write_end);
		free_2(line, eof);
		delete_all_heredoc(stuff);
		exit(EXIT_FAILURE);
	}
	if (write(write_end, tokens->line, ft_strlen(tokens->line)) == -1)
	{
		free_3(tokens->line, tokens, eof);
		delete_all_heredoc(stuff);
		close(write_end);
		exit(EXIT_FAILURE);
	}
	if (write(write_end, "\n", 1) == -1)
	{
		free_3(tokens->line, tokens, eof);
		delete_all_heredoc(stuff);
		close(write_end);
		exit(EXIT_FAILURE);
	}
	free_2(tokens->line, tokens);
}

void	heredoc_no_line(int write_end, char *eof, t_free_close *stuff)
{
	close(write_end);
	free(eof);
	delete_all_heredoc(stuff);
	if (g_signum == SIGINT)
	{
		g_signum = 0;
		exit(SIGINT);
	}
	else
		print_error_dont_set_status(ERROR_HEREDOC_EOF);
	exit(EXIT_SUCCESS);
}

static int	heredoc_child(char *eof, int write_end, t_free_close *stuff, t_infile how_expand)
{
	char	*line;
	int		len_eof;

	set_signal_handler_heredoc();
	len_eof = ft_strlen(eof);
	while (1)
	{
		line = readline("> ");
		if (line == 0)
			heredoc_no_line(write_end, eof, stuff);
		if (ft_strncmp(line, eof, len_eof + 1) == 0)
			break ;
		if (how_expand == INFILE_HEREDOC_EXPAND)
			write_expander_heredoc(line, write_end, stuff, eof);
		else
			write_no_expand_heredoc(line, write_end, stuff, eof);
	}
	delete_all_heredoc(stuff);
	close(write_end);
	free(eof);
	exit(EXIT_SUCCESS);
}

static int	write_heredoc(char *eof, int write_end, t_free_close *stuff, t_infile how_expand)
{
	int	pid;
	int	status;

	set_signal_handler_parent();
	status = 0;
	pid = fork();
	if (pid < 0)
		return (1);
	else if (pid == 0)
		heredoc_child(eof, write_end, stuff, how_expand);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 1)
				perror_set_status(stuff->env, 1, 0);
			return (WEXITSTATUS(status));
		}
		else if (WIFSIGNALED(status))
			return (set_status(stuff->env, WTERMSIG(status)));
	}
	return (1);
}

int	create_heredoc(char *eof, t_free_close *stuff, t_infile how_expand)
{
	int		end[2];

	if (pipe(end) == -1)
		return (-1);
	stuff->fd_read_end = end[0];
	if (write_heredoc(eof, end[1], stuff, how_expand) != 0)
	{
		close(end[1]);
		close(end[0]);
		return (-1);
	}
	close(end[1]);
	return (end[0]);
}
