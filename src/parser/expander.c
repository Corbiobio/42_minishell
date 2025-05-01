/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:02:41 by sflechel          #+#    #+#             */
/*   Updated: 2025/05/01 18:19:51 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include <stddef.h>
#include <stdlib.h>
#include "../../libft/libft.h"

void	tokenize_string(char *line, t_tokenized_line *tokens)
{
	size_t	i;

	*tokens = (t_tokenized_line){.nb_token = ft_strlen(line), .line = line};
	i = 0;
	while (i < tokens->nb_token)
	{
		tokens->tokens[i] = (t_token){.pos = i, .len = 1};
		if (ft_iswhitespace(line[i]))
			tokens->tokens[i].type = TYPE_WHITESPACE;
		else if (line[i] == '|')
			tokens->tokens[i].type = TYPE_PIPE;
		else if (line[i] == '>')
			tokens->tokens[i].type = TYPE_GREATER;
		else if (line[i] == '<')
			tokens->tokens[i].type = TYPE_LESSER;
		else if (line[i] == '\'')
			tokens->tokens[i].type = TYPE_SINGLE_QUOTE;
		else if (line[i] == '"')
			tokens->tokens[i].type = TYPE_DOUBLE_QUOTE;
		else if (line[i] == '$')
			tokens->tokens[i].type = TYPE_DOLLAR;
		else
			tokens->tokens[i].type = TYPE_WORD;
		i++;
	}
}

int	variable_name_start_correctly(t_tokenized_line *line, size_t token_index)
{
	const char	c = line->line[line->tokens[token_index].pos];

	if (ft_isalpha(c))
		return (1);
	if (c == '_')
		return (1);
	return (0);
}

int	turn_quoted_tokens_to_word(t_tokenized_line *line, t_hash_table *env)
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
				if (line->tokens[i].type != TYPE_WORD && line->tokens[i].type
					!= TYPE_WHITESPACE && !(quote_type == TYPE_DOUBLE_QUOTE
						&& line->tokens[i].type == TYPE_DOLLAR))
					line->tokens[i].type = TYPE_DEAD_TOKEN;
				else if (line->tokens[i].type == TYPE_WORD)
					line->tokens[i].type = TYPE_WORD_QUOTED;
				i++;
			}
		}
		if (i++ >= line->nb_token)
			return (print_error_set_status(ERROR_QUOTE_UNCLOSED, env));
	}
	return (0);
}

void	dollar_alone_is_dead(t_tokenized_line *line)
{
	size_t	i;

	i = 0;
	while (i + 1 < line->nb_token)
	{
		if (line->tokens[i].type == TYPE_DOLLAR
			&& !is_word(line->tokens[i + 1]))
			line->tokens[i].type = TYPE_DEAD_TOKEN;
		i++;
	}
	if (i < line->nb_token && line->tokens[i].type == TYPE_DOLLAR)
		line->tokens[i].type = TYPE_DEAD_TOKEN;
}

int	is_allowed_in_variable_name(t_tokenized_line *line, int token_index)
{
	const char	c = line->line[line->tokens[token_index].pos];

	if (ft_isalnum(c))
		return (1);
	if (c == '_')
		return (1);
	return (0);
}
