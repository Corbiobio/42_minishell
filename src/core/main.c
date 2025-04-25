/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:04:17 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/25 10:17:54 by sflechel         ###   ########.fr       */
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
#include <signal.h>
#include <termios.h>
#include <unistd.h>

void	free_cmd_list(t_cmd_list *list)
{
	size_t	i;

	i = 0;
	while (i < list->nb_cmd)
	{
		ft_free_split(list->cmds[i].cmd);
		i++;
	}
	free(list);
}

void	print_hash_table(t_hash_table *table)
{
	size_t	i;

	printf("size: %zu, capacity: %zu\n", table->size, table->capacity);
	i = 0;
	while (i < table->capacity)
	{
		if (table->items[i].key == 0)
			printf("%zu: no value  ", i);
		else if (table->items[i].value == 0)
			printf("%zu: empty/removed  ", i);
		else
			printf("%zu: ,key: %s, value: %s\n", i, table->items[i].key, table->items[i].value);
		i++;
	}
}

int	split_key_value(char *input, char **key, char **value)
{
	size_t	i;

	i = 0;
	while (input[i] && input[i] != '=')
		i++;
	if (!input[i])
		return (1);
	*key = ft_strndup(input, i);
	*value = ft_strdup(&input[i + 1]);
	if (*key == 0 || *value == 0)
	{
		free(*key);
		free(*value);
		return (1);
	}
	return (0);
}

t_hash_table	*convert_env_to_table(char **env)
{
	t_hash_table	*env_table;
	char			*key;
	char			*value;
	size_t			i;

	env_table = table_alloc(150);
	if (env_table == 0)
		return (0);
	i = 0;
	while (env[i])
	{
		if (split_key_value(env[i], &key, &value) == 1)
		{
			table_delete_table(env_table);
			return (0);
		}
		table_insert(env_table, key, value);
		i++;
	}
	return (env_table);
}

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



int	main(int ac, char **av, char **env)
{
	int				status;
	t_cmd_list		*list;
	t_hash_table	*env_table;
	char			*line;

	set_signal_handler();
	env_table = convert_env_to_table(env);
	if (env_table == 0)
		return (1);
	while (42)
	{
		line = readline("beurre_demishell$ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		list = parser(line, env_table);
		create_child_and_exec_cmd(list, env_table);
		while (wait(&status) > 0)
			;
		free_cmd_list(list);
	}
	rl_clear_history();
	table_delete_table(env_table);
	return (status);
	(void)ac;
	(void)av;
}

//main without parsing and parsing print
//int	main(int ac, char **av, char **env)
//{
//	t_cmd_list		*cmd_list = malloc(sizeof(t_cmd_list) + 3 * sizeof(t_cmd));
//	t_hash_table	*env_table;
//	int				status = 0;

//	env_table = convert_env_to_table(env);
//	if (env_table == 0)
//		return (1);
//	//int in = open("in", O_RDWR);
//	//int out = open("out", O_RDWR);
//	cmd_list->cmds[0] = (t_cmd){ {-2, -2},(char*[]){"ecsho","test", NULL}};
//	cmd_list->cmds[1] = (t_cmd){ {-2, -2},(char*[]){"cat", "-e",NULL}};
//	cmd_list->nb_cmd = 2;
//	create_child_and_exec_cmd(cmd_list, env_table);
//	while (wait(&status) > 0)
//		;
//	table_delete_table(env_table);
//	//close(in);
//	//close(out);
//	free(cmd_list);
//	return (status);
//	(void)ac;
//	(void)av;
//}
