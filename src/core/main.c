/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:04:17 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/01 17:49:00 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/execution.h"
#include "../../includes/parser.h"
#include "../../libft/libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <termios.h>

void	free_cmd_list(t_cmd_list *list)
{
	size_t	i;

	i = 0;
	while (i < list->nb_cmd)
	{
		if (list->cmds[i].cmd)
			ft_free_split(list->cmds[i].cmd);
		i++;
	}
	free(list);
}

struct termios	command_loop(t_hash_table *env)
{
	struct termios	old_termios;
	char			*line;
	t_cmd_list		*list;

	tcgetattr(STDIN_FILENO, &old_termios);
	while (42)
	{
		printf("echo $?: %s\n", table_search(env, "?"));
		set_signal_handler_main(old_termios);
		line = readline("beurre_demishell$ ");
		if (!line)
			return (old_termios);
		if (*line)
			add_history(line);
		list = parser(line, env);
		if (!list)
			continue ;
		if (exec_cdms_list(list, env, old_termios) == 42)
		{
			free_cmd_list(list);
			return (old_termios);
		}
		free_cmd_list(list);
	}
}

int	main(int ac, char **av, char **env)
{
	t_hash_table	*env_table;
	struct termios	old_termios;
	int				status;

	status = 1;
	if (ac > 1)
		return (EXIT_FAILURE);
	env_table = convert_env_to_table(env);
	if (env_table == 0)
		return (12);
	old_termios = command_loop(env_table);
	rl_clear_history();
	safe_atoi(table_search(env_table, "?"), &status);
	printf("%i exited main\n", status);
	table_delete_table(env_table);
	tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
	write(STDERR_FILENO, "exit\n", 5);
	return (status);
	(void)av;
}
