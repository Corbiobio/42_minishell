/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:40:46 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/09 15:09:31 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	expand_variables(t_tokenized_line *input,
				t_tokenized_line *intermediary, t_hash_table *env)
{
	char	*new_line;
	t_token	token;
	size_t	i;

	fuse_dollars(input, intermediary);
	i = 0;
	while (i < intermediary->nb_token)
	{
		token = intermediary->tokens[i];
		if (intermediary->tokens[i].type == TYPE_DOLLAR
			&& intermediary->tokens[i].len == 1)
			intermediary->tokens[i].type = TYPE_DEAD_TOKEN;
		i++;
	}
	new_line = search_and_replace(intermediary, env);
	if (new_line == 0)
		return (1);
	return (0);
}

int	is_token_allowed_in_word(t_token token)
{
	if (is_word(token))
		return (1);
	if (token.type == TYPE_DEAD_TOKEN)
		return (1);
	if (token.type == TYPE_SINGLE_QUOTE)
		return (1);
	if (token.type == TYPE_DOUBLE_QUOTE)
		return (1);
	if (token.type == TYPE_DOLLAR)
		return (1);
	return (0);
}

void	prevent_expand_in_eof(t_tokenized_line *line)
{
	size_t	i;

	i = 0;
	while (i + 1 < line->nb_token)
	{
		if (line->tokens[i].type != TYPE_LESSER
			|| line->tokens[i + 1].type != TYPE_LESSER)
		{
			i++;
			continue ;
		}
		i += 2;
		while (i < line->nb_token && line->tokens[i].type == TYPE_WHITESPACE)
			i++;
		while (i < line->nb_token && is_token_allowed_in_word(line->tokens[i]))
		{
			if (i < line->nb_token && line->tokens[i].type == TYPE_DOLLAR)
				line->tokens[i].type = TYPE_DEAD_TOKEN;
			i++;
		}
		i++;
	}
}

t_tokenized_line	*expander(char *line, t_hash_table *env)
{
	void			*tokens;
	void			*tokens_output;
	const size_t	len_line = ft_strlen(line);
	const size_t	size = sizeof(t_tokenized_line)
		+ sizeof(t_token) * len_line;

	tokens = malloc(size * 2);
	if (tokens == 0)
		return (0);
	tokens_output = malloc(size);
	if (tokens_output == 0)
		return (free_1_return_null(tokens));
	tokenize_string(line, tokens);
	if (turn_quoted_tokens_to_word(tokens, env) == 1)
		return (free_2_return_null(tokens, tokens_output));
	quotes_to_empty_words(tokens, tokens + size);
	dollar_alone_is_dead(tokens + size);
	prevent_expand_in_eof(tokens + size);
	if (expand_variables(tokens + size, tokens_output, env) == 1)
		return (free_2_return_null(tokens, tokens_output));
	free(tokens);
	return (tokens_output);
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
				add_token(output, (t_token){.pos
					= input->tokens[i].pos + j, .len = 1});
				if (ft_iswhitespace(input->line[input->tokens[i].pos + j]))
					change_last_token_type(output, TYPE_WHITESPACE);
				else
					change_last_token_type(output, TYPE_WORD);
				j++;
			}
			i++;
			continue ;
		}
		add_token(output, input->tokens[i++]);
	}
}
