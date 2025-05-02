/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:50:35 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/02 17:45:28 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include "libft.h"
#include "parser.h"

volatile sig_atomic_t	g_signum;

void	reset_signum_save_status(t_hash_table *env)
{
	g_signum = 0;
	table_insert(env, ft_strdup("?"), ft_strdup("130"));
}

static void	signal_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		close(STDIN_FILENO);
		g_signum = SIGINT;
	}
}

void	set_signal_handler_heredoc(void)
{
	struct sigaction	sigset;

	if (sigemptyset(&sigset.sa_mask) == -1)
		return ;
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

void	set_signal_handler_parent(void)
{
	struct sigaction	sigset;

	if (sigemptyset(&sigset.sa_mask) == -1)
		return ;
	sigaddset(&sigset.sa_mask, SIGINT);
	sigset.sa_flags = SA_RESTART;
	sigset.sa_handler = &signal_handler_parent;
	sigaction(SIGINT, &sigset, 0);
}
