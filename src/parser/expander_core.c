/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:40:46 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/30 08:33:59 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	expand_variables(t_tokenized_line *input,
				t_tokenized_line *intermediary, t_hash_table *env)
{
	char	*new_line;

	fuse_dollars(input, intermediary);
	new_line = search_and_replace(intermediary, env);
	if (new_line == 0)
		return (1);
	return (0);
}

t_tokenized_line	*expander(char *line, t_hash_table *env)
{
	void			*tokens;
	void			*tokens_output;
	const size_t	len_line = ft_strlen(line);
	const size_t	size = sizeof(t_tokenized_line)
		+ sizeof(t_token) * len_line;

	tokens = malloc(size);
	if (tokens == 0)
		return (0);
	tokens_output = malloc(size);
	if (tokens_output == 0)
		return (free_1_return_null(tokens));
	tokenize_string(line, tokens);
	if (turn_quoted_tokens_to_word(tokens, env) == 1)
		return (free_2_return_null(tokens, tokens_output));
	dollar_alone_is_dead(tokens);
	if (expand_variables(tokens, tokens_output, env) == 1)
		return (free_2_return_null(tokens, tokens_output));
	free(tokens);
	return (tokens_output);
}

void	change_last_token_type(t_tokenized_line *line, t_type type)
{
	line->tokens[line->nb_token - 1].type = type;
}

void	expand_token_list(t_tokenized_line *input, t_tokenized_line *output)
{
	size_t	i;
	size_t	j;

	i = 0;
	*output = (t_tokenized_line){.line = input->line};
	print_tokens(input);
	while (i < input->nb_token)
	{
		print_tokens(output);
		if (input->tokens[i].type == TYPE_DOLLAR)
		{
			j = 0;
			while (j < input->tokens[i].len)
			{
				add_token(output, (t_token){.pos = input->tokens[i].pos + j, .len = 1});
				print_tokens(output);
				if (ft_iswhitespace(input->line[input->tokens[i].pos + j]))
					change_last_token_type(output, TYPE_WHITESPACE);
				else
					change_last_token_type(output, TYPE_WORD);
				print_tokens(output);
				j++;
			}
			i++;
			continue ;
		}
		add_token(output, input->tokens[i]);
		i++;
	}
}
