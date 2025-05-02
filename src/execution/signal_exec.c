/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:10:38 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/02 12:25:18 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bits/posix_opt.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

void	signal_handler_exec(int signum)
{
	write(STDIN_FILENO, "\n", 1);
	(void)signum;
}

void	sigcatch(int signum)
{
	write(STDIN_FILENO, "Quit (core dumped)\n", 19);
	(void)signum;
}

void	set_signal_handler_exec(struct termios old_termios)
{
	struct sigaction	sigset;

	tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
	if (sigemptyset(&sigset.sa_mask) == -1)
		return ;
	sigaddset(&sigset.sa_mask, SIGINT);
	sigset.sa_flags = SA_RESTART;
	sigset.sa_handler = &signal_handler_exec;
	sigaction(SIGINT, &sigset, 0);
	if (sigemptyset(&sigset.sa_mask) == -1)
		return ;
	sigaddset(&sigset.sa_mask, SIGQUIT);
	sigset.sa_flags = SA_RESTART;
	sigset.sa_handler = &sigcatch;
	sigaction(SIGQUIT, &sigset, 0);
	if (sigemptyset(&sigset.sa_mask) == -1)
		return ;
	sigaddset(&sigset.sa_mask, SIGPIPE);
	sigset.sa_flags = SA_RESTART;
	sigset.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &sigset, 0);
}
