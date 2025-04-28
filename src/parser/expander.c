/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:02:41 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/28 19:09:39 by sflechel         ###   ########.fr       */
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
	ft_strlcat(new_line, line, dollar.pos + 1);
	if (s2)
		ft_strlcat(new_line, s2, len);
	ft_strlcat(new_line, &line[dollar.pos + dollar.len], len);
	return (new_line);
}

void	fuse_dollars(t_tokenized_line *input, t_tokenized_line *output)
{
	size_t	i;

	i = 0;
	*output = (t_tokenized_line){.line = input->line};
	while (i < input->nb_token)
	{
		add_token(output, input->tokens[i]);
		if (input->tokens[i].type == TYPE_DOLLAR)
		{
			i++;
			while (i < input->nb_token && is_word(input->tokens[i]))
			{
				output->tokens[output->nb_token - 1].len++;
				i++;
			}
			continue ;
		}
		i++;
	}
}
#include <stdio.h>
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

void	correct_positions(t_tokenized_line *line, size_t new_len, size_t old_len, size_t index)
{
	size_t	i;

	i = index + 1;
	while (i < line->nb_token)
	{
		line->tokens[i].pos -= old_len;
		line->tokens[i].pos += new_len;
		i++;
	}
}

char	*search_and_replace(t_tokenized_line *line, t_hash_table *env)
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
			correct_positions(line, ft_strlen(replacement), line->tokens[i].len, i);
			line->tokens[i].len = ft_strlen(replacement);
			free(line->line);
			line->line = new_line;
		}
		i++;
	}
	return (line->line);
}

void	expand_token_list(t_tokenized_line *input, t_tokenized_line *output)
{
	size_t	i;
	size_t	j;

	i = 0;
	*output = (t_tokenized_line){.line = input->line};
	while (i < input->nb_token)
	{
		if (input->tokens[i].type == TYPE_DOLLAR)
		{
			j = 0;
			while (j < input->tokens[i].len)
			{
				output->tokens[i + j] = (t_token){.pos = i + j, .len = 1};
				if (ft_iswhitespace(input->line[input->tokens[i].pos + j]))
					output->tokens[i + j].type = TYPE_WHITESPACE;
				else
					output->tokens[i + j].type = TYPE_WORD;
				output->nb_token++;
				j++;
			}
			i++;
			continue ;
		}
		add_token(output, input->tokens[i]);
		i++;
	}
}

void	expand_variables(t_tokenized_line *input, t_tokenized_line *intermediary, t_hash_table *env)
{
	char	*new_line;

	fuse_dollars(input, intermediary);
	new_line = search_and_replace(intermediary, env);
}
