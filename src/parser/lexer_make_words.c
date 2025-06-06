/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_make_words.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:45:40 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/19 10:34:36 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_token	*last_token(t_tokenized_line *line)
{
	return (&line->tokens[line->nb_token - 1]);
}

int	remove_empty_words(t_tokenized_line *input, t_tokenized_line *output)
{
	size_t	i;

	i = 0;
	*output = (t_tokenized_line){.line = input->line};
	while (i < input->nb_token)
	{
		if (is_word(input->tokens[i]) && input->tokens[i].len == 0)
		{
			output->line = line_surgery(input->line, input->tokens[i], 2);
			if (output->line == 0)
				return (1);
			free(input->line);
			input->line = output->line;
			correct_positions(input, 0, 2, i);
		}
		add_token(output, input->tokens[i]);
		i++;
	}
	return (0);
}

void	fuse_words(t_tokenized_line *input, t_tokenized_line *output)
{
	size_t	i;

	*output = (t_tokenized_line){.line = input->line};
	i = 0;
	while (i < input->nb_token)
	{
		add_token(output, input->tokens[i]);
		if (i < input->nb_token && is_word(input->tokens[i]))
		{
			i++;
			while (i < input->nb_token && is_word(input->tokens[i]))
			{
				if (last_token(output)->len == 0 && input->tokens[i].len > 0)
					last_token(output)->pos = input->tokens[i].pos;
				if (input->tokens[i].type == TYPE_WORD_QUOTED)
					output->tokens[output->nb_token - 1].type
						= TYPE_WORD_QUOTED;
				last_token(output)->len += input->tokens[i].len;
				i++;
			}
		}
		else
			i++;
	}
}

void	remove_whitespaces(t_tokenized_line *input, t_tokenized_line *output)
{
	size_t	i;

	i = 0;
	*output = (t_tokenized_line){.line = input->line};
	while (i < input->nb_token)
	{
		if (input->tokens[i].type != TYPE_WHITESPACE)
			add_token(output, input->tokens[i]);
		i++;
	}
}

void	fuse_chevrons(t_tokenized_line *in, t_tokenized_line *out)
{
	size_t	i;

	i = 0;
	*out = (t_tokenized_line){.line = in->line};
	while (i + 1 < in->nb_token)
	{
		add_token(out, in->tokens[i]);
		if (in->tokens[i].type == TYPE_GREATER
			&& in->tokens[i + 1].type == TYPE_GREATER)
		{
			out->tokens[out->nb_token - 1].type = TYPE_GREATER_GREATER;
			out->tokens[out->nb_token - 1].len = 2;
			i++;
		}
		else if (in->tokens[i].type == TYPE_LESSER
			&& in->tokens[i + 1].type == TYPE_LESSER)
		{
			out->tokens[out->nb_token - 1].type = TYPE_LESSER_LESSER;
			out->tokens[out->nb_token - 1].len = 2;
			i++;
		}
		i++;
	}
	if (i < in->nb_token)
		add_token(out, in->tokens[i]);
}
