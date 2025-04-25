/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:55:37 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/25 15:10:07 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

void	letsdonothing(int signum)
{
	(void)signum;
}

void	set_signal_handler_exec(void)
{
	struct sigaction	sigset;

	sigemptyset(&sigset.sa_mask);
	sigaddset(&sigset.sa_mask, SIGINT);
	sigaddset(&sigset.sa_mask, SIGQUIT);
	sigset.sa_flags = 0;
	sigset.sa_handler = &letsdonothing;
	sigaction(SIGINT, &sigset, 0);
	sigaction(SIGQUIT, &sigset, 0);
}
