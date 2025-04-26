/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:10:38 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/26 17:05:10 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bits/posix_opt.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

void	signal_handler_exec(int signum)
{
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

	sigemptyset(&sigset.sa_mask);
	sigaddset(&sigset.sa_mask, SIGINT);
	sigset.sa_flags = 0;
	sigset.sa_handler = &signal_handler_exec;
	sigaction(SIGINT, &sigset, 0);
	tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
	sigemptyset(&sigset.sa_mask);
	sigaddset(&sigset.sa_mask, SIGINT);
	sigset.sa_flags = 0;
	sigset.sa_handler = &sigcatch;
	sigaction(SIGQUIT, &sigset, 0);
}
