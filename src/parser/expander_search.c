/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_search.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:10:49 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/01 18:19:55 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include <stddef.h>

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

char	*search_in_env(t_tokenized_line *input,
					size_t word_pos, t_hash_table *env)
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

void	correct_positions(t_tokenized_line *line, size_t new_len,
					size_t old_len, size_t index)
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
			new_line = token_line_triple_join(line->line,
					line->tokens[i], replacement);
			if (new_line == 0)
				return (0);
			correct_positions(line, ft_strlen(replacement),
				line->tokens[i].len, i);
			line->tokens[i].len = ft_strlen(replacement);
			free(line->line);
			line->line = new_line;
		}
		i++;
	}
	return (line->line);
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
			if (i < input->nb_token && input->line[input->tokens[i].pos] == '?')
				output->tokens[output->nb_token - 1].len++;
			else if (variable_name_start_correctly(input, i))
			{
				while (i < input->nb_token
					&& is_allowed_in_variable_name(input, i))
				{
					i++;
					output->tokens[output->nb_token - 1].len++;
				}
				continue ;
			}
		}
		i++;
	}
}
