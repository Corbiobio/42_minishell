/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:04:17 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/27 18:41:06 by sflechel         ###   ########.fr       */
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

int	main(int ac, char **av, char **env)
{
	int				status;
	t_cmd_list		*list;
	t_hash_table	*env_table;
	char			*line;
	struct termios	old_termios;

	status = 0;
	env_table = convert_env_to_table(env);
	if (env_table == 0)
		return (1);
	status = 0;
	while (42)
	{
		old_termios = set_signal_handler_main();
		line = readline("beurre_demishell$ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		list = parser(line, env_table);
		if (!list)
			continue ;
		if (list->nb_cmd == 1 && list->cmds[0].cmd[0] != NULL)
		{
			create_child_and_exec_cmd(list, env_table, old_termios);
			while (wait(&status) > 0)
				;
		}
		free_cmd_list(list);
	}
	rl_clear_history();
	table_delete_table(env_table);
	tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
	return (status);
	(void)ac;
	(void)av;
}
