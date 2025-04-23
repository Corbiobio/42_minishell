/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:02:41 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/23 16:43:54 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include <stddef.h>
#include <stdlib.h>
#include "../../libft/libft.h"

static char	*token_line_triple_join(char *line, t_token dollar, char *s2)
{
	char	*new_line;
	size_t	len;

	len = ft_strlen(line) - dollar.len + ft_strlen(s2) + 1;
	new_line = ft_calloc(len, sizeof(char));
	if (new_line <= 0)
		return (0);
	ft_strlcat(new_line, line, dollar.pos);
	ft_strlcat(new_line, s2, len);
	ft_strlcat(new_line, &line[dollar.pos + dollar.len], len);
	return (new_line);
}
#include <stdio.h>
void	fuse_dollars(t_tokenized_line *input, t_tokenized_line *output)
{
	size_t	i;

	i = 0;
	*output = (t_tokenized_line){.line = input->line};
	while (i < input->nb_token)
	{
		printf("%zu: pos\n", input->tokens[i].pos);
		printf("%zu: nb_tokens\n", input->nb_token);
		add_token(output, input->tokens[i]);
		if (input->tokens[i].type == TYPE_DOLLAR)
		{
			i++;
			while (i < input->nb_token && input->tokens[i].type == TYPE_WORD)
			{
				output->tokens[output->nb_token - 1].len++;
				i++;
			}
			add_token(output, input->tokens[i]);
		}
		i++;
	}
}

char	*search_in_env(t_tokenized_line *input, size_t word_pos, t_hash_table *env)
{
	const size_t	pos = input->tokens[word_pos].pos + 1;
	const size_t	len = input->tokens[word_pos].len - 1;
	char			*key;
	char			*value;

	key = ft_strndup(&input->line[pos], len);
	if (key == 0)
		return (0);
	value = table_search(env, key);
	free(key);
	return (value);
}

void	search_and_replace(t_tokenized_line *line, t_hash_table *env)
{
	size_t	i;
	char	*replacement;
	char	*new_line;

	i = 0;
	while (i < line->nb_token)
	{
		if (line->tokens[i].type == TYPE_DOLLAR)
		{
			replacement = search_in_env(line, i, env);
			new_line = token_line_triple_join(line->line, line->tokens[i], replacement);
			free(replacement);
			free(line->line);
			line->line = new_line;
		}
		i++;
	}
}

void	expand_variables(t_tokenized_line *input, t_tokenized_line *output, t_hash_table *env)
{
	t_tokenized_line	*intermediary;

	intermediary = malloc(sizeof(t_tokenized_line) + sizeof(t_token) * input->nb_token);
	if (intermediary == 0)
		return ;
	fuse_dollars(input, intermediary);
	search_and_replace(intermediary, env);
	tokenize_string(intermediary->line, output);
	free(intermediary);
}
