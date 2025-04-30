/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:50:35 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/30 17:51:14 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include "parser.h"

volatile sig_atomic_t	g_signum;

static void	signal_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		printf("sigint!\n");
		close(STDIN_FILENO);
		g_signum = SIGINT;
		printf("g_signum: %i\n", g_signum);
	}
}

void	set_signal_handler_heredoc(void)
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

void	set_signal_handler_parent(void)
{
	struct sigaction	sigset;

	sigemptyset(&sigset.sa_mask);
	sigaddset(&sigset.sa_mask, SIGINT);
	sigset.sa_flags = SA_RESTART;
	sigset.sa_handler = &signal_handler_parent;
	sigaction(SIGINT, &sigset, 0);
}
