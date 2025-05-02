/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 08:53:58 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/02 14:10:45 by sflechel         ###   ########.fr       */
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

extern volatile sig_atomic_t	g_signum;

void	delete_all_heredoc(t_free_close *stuff)
{
	close(stuff->fd_read_end);
	free(stuff->line1);
	free(stuff->line2);
	free(stuff->cmds);
	table_delete_table(stuff->env);
	free(stuff);
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

static int	heredoc_child(char *eof, int write_end,
					t_free_close *stuff, t_infile how_expand)
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
			write_no_expand_heredoc(line, write_end);
	}
	delete_all_heredoc(stuff);
	close(write_end);
	free(eof);
	exit(EXIT_SUCCESS);
}

static int	fork_heredoc(char *eof, int write_end,
						t_free_close *stuff, t_infile how_expand)
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
			if (WEXITSTATUS(status) == SIGINT)
				return (set_status(stuff->env, SIGINT));
			return (WEXITSTATUS(status));
		}
		else if (WIFSIGNALED(status))
			return (set_status(stuff->env, WTERMSIG(status) + 128));
	}
	return (1);
}

int	create_heredoc(char *eof, t_free_close *stuff, t_infile how_expand)
{
	int		end[2];

	if (pipe(end) == -1)
		return (-1);
	stuff->fd_read_end = end[0];
	if (fork_heredoc(eof, end[1], stuff, how_expand) != 0)
	{
		close(end[1]);
		close(end[0]);
		return (-1);
	}
	close(end[1]);
	return (end[0]);
}
