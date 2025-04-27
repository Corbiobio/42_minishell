/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 09:25:42 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/27 14:32:16 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include "../../libft/libft.h"
#include "../../includes/minishell.h"
#include <stddef.h>
#include <stdio.h>
#include <sys/wait.h>

int	count_commands(t_tokenized_line *line)
{
	size_t	i;
	int		nb_commands;

	i = 0;
	nb_commands = 1;
	while (i < line->nb_token)
	{
		if (line->tokens[i].type == TYPE_PIPE)
			nb_commands++;
		i++;
	}
	return (nb_commands);
}

void	init_cmds(t_cmd_list *cmds, size_t count_commands)
{
	size_t	i;

	cmds->nb_cmd = count_commands;
	i = 0;
	while (i < count_commands)
	{
		cmds->cmds[i] = (t_cmd){.io[0] = -2, .io[1] = -2};
		i++;
	}
}

void	print_cmds(t_cmd_list *list)
{
	size_t	i;
	int		j;

	printf("nb_cmd: %zu\n", list->nb_cmd);
	i = 0;
	while (i < list->nb_cmd)
	{
		printf("i: %i, o: %i\n", list->cmds[i].io[0], list->cmds[i].io[1]);
		j = 0;
		while (list->cmds[i].cmd[j] != 0)
		{
			printf("%s, ", list->cmds[i].cmd[j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

void	word_single_quotes_to_word(t_tokenized_line *tokens)
{
	size_t	i;

	i = 0;
	while (i < tokens->nb_token)
	{
		if (tokens->tokens[i].type == TYPE_WORD_SINGLE_QUOTE)
			tokens->tokens[i].type = TYPE_WORD;
		i++;
	}
}

t_cmd_list	*parser(char *line, t_hash_table *env)
{
	t_tokenized_line	*raw;
	t_tokenized_line	*expanded;
	t_cmd_list			*cmds;

	raw = expander(line, env);
	if (raw == 0)
		return (0);
	expanded = lexer(raw);
	if (expanded == 0)
		return (0);
	cmds = malloc(sizeof(t_cmd_list) + sizeof(t_cmd) * count_commands(expanded));
	if (cmds == 0)
		return (free_3_return_null(raw, expanded, expanded->line));
	init_cmds(cmds, count_commands(expanded));
	if (open_infile_outfile(expanded, cmds) != 0)
		return (free_4_return_null(raw, expanded, expanded->line, cmds));
	if (grammarify(expanded, cmds) == 1)
	{
		free_cmd_list(cmds);
		return (free_3_return_null(raw, expanded, expanded->line));
	}
	free_3(raw, expanded->line, expanded);
	return (cmds);
}
