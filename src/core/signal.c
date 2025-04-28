/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:25:36 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/28 16:49:52 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bits/posix_opt.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

void	signal_handler_shell(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

struct termios	set_signal_handler_main(void)
{
	struct sigaction	sigset;
	struct termios		old_termios;
	struct termios		new_termios;

	sigemptyset(&sigset.sa_mask);
	sigaddset(&sigset.sa_mask, SIGINT);
	sigset.sa_flags = SA_RESTART;
	sigset.sa_handler = &signal_handler_shell;
	sigaction(SIGINT, &sigset, 0);
	tcgetattr(STDIN_FILENO, &old_termios);
	new_termios = old_termios;
	new_termios.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
	return (old_termios);
}
