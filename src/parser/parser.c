/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 09:25:42 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/29 18:07:57 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include "../../libft/libft.h"
#include "../../includes/minishell.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int	count_cmds(t_tokenized_line *line)
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

t_cmd_list	*parser(char *line, t_hash_table *env)
{
	t_free_close		*to_free;
	t_tokenized_line	*raw;
	t_tokenized_line	*expanded;
	t_cmd_list			*cmds;

	raw = expander(line, env);
	if (raw == 0)
		return (0);
	expanded = lexer(raw);
	if (expanded == 0)
		return (free_2_return_null(raw->line, raw));
	free(raw);
	cmds = malloc(sizeof(t_cmd_list) + sizeof(t_cmd) * count_cmds(expanded));
	to_free = malloc(sizeof(t_free_close));
	if (cmds == 0 || to_free == 0)
		return (free_4_return_null(cmds, to_free, expanded->line, expanded));
	*to_free = (t_free_close){0, expanded, expanded->line, cmds, env};
	init_cmds(cmds, count_cmds(expanded));
	if (open_infile_outfile(expanded, cmds, to_free) != 0)
		return (free_4_return_null(to_free, expanded->line, expanded, cmds));
	if (grammarify(expanded, cmds) == 1)
		return (free_3_return_null(expanded->line, expanded, to_free));
	free_3(expanded->line, expanded, to_free);
	return (cmds);
}
