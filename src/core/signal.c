/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:25:36 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/02 17:34:04 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bits/posix_opt.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

extern volatile sig_atomic_t	g_signum;

void	signal_handler_shell(int signum)
{
	if (signum == SIGINT)
	{
		g_signum = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_signal_handler_main(struct termios old_termios)
{
	struct sigaction	sigset;
	struct termios		new_termios;

	if (sigemptyset(&sigset.sa_mask) == -1)
		return ;
	sigaddset(&sigset.sa_mask, SIGINT);
	sigset.sa_flags = SA_RESTART;
	sigset.sa_handler = &signal_handler_shell;
	sigaction(SIGINT, &sigset, 0);
	new_termios = old_termios;
	new_termios.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}
