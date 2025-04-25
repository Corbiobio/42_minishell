/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:25:36 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/25 11:05:13 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void	signal_handler_shell(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
		//write(STDIN_FILENO, "  \b\b", 4);
	}
}

void	set_signal_handler(void)
{
	struct sigaction	sigset;

	sigemptyset(&sigset.sa_mask);
	sigaddset(&sigset.sa_mask, SIGINT);
	sigaddset(&sigset.sa_mask, SIGQUIT);
	sigset.sa_flags = 0;
	sigset.sa_handler = &signal_handler_shell;
	sigaction(SIGINT, &sigset, 0);
	sigaction(SIGQUIT, &sigset, 0);
}
