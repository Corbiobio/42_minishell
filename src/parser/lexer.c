/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:25:03 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/15 17:26:32 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	turn_whitespaces_to_word(t_tokenized_line *line)
{
	size_t		i;
	t_type		quote_type;

	i = 0;
	while (i < line->nb_token)
	{
		if (is_quote(line->tokens[i]))
		{
			quote_type = line->tokens[i].type;
			i++;
			while (i < line->nb_token && line->tokens[i].type != quote_type)
			{
				if (line->tokens[i].type == TYPE_WHITESPACE)
					line->tokens[i].type = TYPE_WORD;
				i++;
			}
		}
		i++;
	}
}

void	turn_dead_to_word(t_tokenized_line *line)
{
	size_t	i;

	i = 0;
	while (i < line->nb_token)
	{
		if (line->tokens[i].type == TYPE_DEAD_TOKEN)
			line->tokens[i].type = TYPE_WORD_QUOTED;
		i++;
	}
}

char	*line_surgery(char *line, t_token quote, int amount)
{
	const size_t	len_line = ft_strlen(line);
	char			*new_line;

	new_line = ft_calloc(len_line, sizeof(char));
	if (new_line == 0)
		return (0);
	ft_strlcat(new_line, line, quote.pos + 1);
	ft_strlcat(new_line, &line[quote.pos + amount], len_line);
	return (new_line);
}

int	remove_quotes(t_tokenized_line *input, t_tokenized_line *output)
{
	size_t	i;

	i = 0;
	*output = (t_tokenized_line){.line = input->line};
	while (i < input->nb_token)
	{
		if (is_quote(input->tokens[i]))
		{
			output->line = line_surgery(input->line, input->tokens[i], 1);
			if (output->line == 0)
				return (1);
			free(input->line);
			input->line = output->line;
			correct_positions(input, 0, 1, i);
			i++;
			continue ;
		}
		add_token(output, input->tokens[i]);
		i++;
	}
	return (0);
}

t_tokenized_line	*lexer(t_tokenized_line *input)
{
	void			*tokens;
	void			*tokens_output;
	const size_t	len_line = ft_strlen(input->line);
	const size_t	size = sizeof(t_tokenized_line)
		+ sizeof(t_token) * len_line;

	tokens = malloc(size * 5);
	if (tokens == 0)
		return (0);
	tokens_output = malloc(size);
	if (tokens_output == 0)
		return (free_1_return_null(tokens));
	expand_token_list(input, tokens);
	turn_whitespaces_to_word(tokens);
	turn_dead_to_word(tokens);
	if (remove_quotes(tokens, tokens + size) == 1)
		return (free_2_return_null(tokens, tokens_output));
	if (remove_empty_words(tokens + size, tokens + size * 2) == 1)
		return (free_2_return_null(tokens, tokens_output));
	fuse_words(tokens + size * 2, tokens + size * 3);
	fuse_chevrons(tokens + size * 3, tokens + size * 4);
	remove_whitespaces(tokens + size * 4, tokens_output);
	free(tokens);
	return (tokens_output);
}
