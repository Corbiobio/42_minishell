/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:25:03 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/28 19:18:05 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include "../../libft/libft.h"
#include "minishell.h"
#include <stddef.h>
#include <stdio.h>

int	is_token_character(char c)
{
	if (c == '|')
		return (0);
	if (c == '<')
		return (0);
	if (c == '>')
		return (0);
	return (1);
}

int	is_word(t_token token)
{
	if (token.type == TYPE_WORD)
		return (1);
	else if (token.type == TYPE_WORD_QUOTED)
		return (1);
	return (0);
}

int	is_quote(t_token token)
{
	if (token.type == TYPE_SINGLE_QUOTE)
		return (1);
	else if (token.type == TYPE_DOUBLE_QUOTE)
		return (1);
	return (0);
}

void	add_token(t_tokenized_line *line, t_token token_to_add)
{
	line->tokens[line->nb_token] = token_to_add;
	line->nb_token++;
}

char	*type_to_str(t_type type)
{
	if (type == TYPE_ERROR)
		return ("error");
	if (type == TYPE_WORD)
		return ("word");
	if (type == TYPE_DEAD_TOKEN)
		return ("dead token");
	if (type == TYPE_WHITESPACE)
		return ("whitespace");
	if (type == TYPE_SINGLE_QUOTE)
		return ("single_quote");
	if (type == TYPE_DOUBLE_QUOTE)
		return ("double_quote");
	if (type == TYPE_GREATER)
		return ("greater");
	if (type == TYPE_LESSER)
		return ("lesser");
	if (type == TYPE_GREATER_GREATER)
		return ("greater_greater");
	if (type == TYPE_LESSER_LESSER)
		return ("lesser_lesser");
	if (type == TYPE_PIPE)
		return ("pipe");
	if (type == TYPE_DOLLAR)
		return ("dollar");
	if (type == TYPE_WORD_QUOTED)
		return ("quoted word");
	return ("error");
}

void	print_tokens(t_tokenized_line *line)
{
	size_t	i;

	printf("line: %s\n nb_tokens: %zu\n", line->line, line->nb_token);
	i = 0;
	while (i < line->nb_token)
	{
		printf("[ %.*s ] type: %s\n", (int)line->tokens[i].len, line->line + line->tokens[i].pos, type_to_str(line->tokens[i].type));
		i++;
	}
}

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

void	dollar_alone_is_dead(t_tokenized_line *line)
{
	size_t	i;

	i = 0;
	while (i + 1 < line->nb_token)
	{
		if (line->tokens[i].type == TYPE_DOLLAR && !is_word(line->tokens[i + 1]))
			line->tokens[i].type = TYPE_DEAD_TOKEN;
		i++;
	}
	if (i < line->nb_token && line->tokens[i].type == TYPE_DOLLAR)
		line->tokens[i].type = TYPE_DEAD_TOKEN;
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

int	turn_quoted_tokens_to_word(t_tokenized_line *line)
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
			if (i++ >= line->nb_token)
				return (print_error_return_one(ERROR_QUOTE_UNCLOSED));
		}
		i++;
	}
	return (0);
}

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
			if (i >= line->nb_token)
				printf("ERROR_QUOTE");
			i++;
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

void	fuse_words(t_tokenized_line *input, t_tokenized_line *output)
{
	size_t	i;

	*output = (t_tokenized_line){.line = input->line};
	i = 0;
	while (i < input->nb_token)
	{
		add_token(output, input->tokens[i]);
		if (input->tokens[i].type == TYPE_WORD)
		{
			i++;
			while (i < input->nb_token && is_word(input->tokens[i]))
			{
				if (input->tokens[i].type == TYPE_WORD_QUOTED)
					output->tokens[output->nb_token - 1].type = TYPE_WORD_QUOTED;
				output->tokens[output->nb_token - 1].len++;
				i++;
			}
			if (i < input->nb_token)
				add_token(output, input->tokens[i]);
		}
		i++;
	}
}

void	fuse_chevrons(t_tokenized_line *input, t_tokenized_line *output)
{
	size_t	i;

	i = 0;
	*output = (t_tokenized_line){.line = input->line};
	while (i < input->nb_token)
	{
		add_token(output, input->tokens[i]);
		if (i + 1 < input->nb_token)
		{
			if (input->tokens[i].type == TYPE_GREATER && input->tokens[i + 1].type == TYPE_GREATER)
			{
				output->tokens[output->nb_token - 1].type = TYPE_GREATER_GREATER;
				output->tokens[output->nb_token - 1].len = 2;
				i++;
			}
			else if (input->tokens[i].type == TYPE_LESSER && input->tokens[i + 1].type == TYPE_LESSER)
			{
				output->tokens[output->nb_token - 1].type = TYPE_LESSER_LESSER;
				output->tokens[output->nb_token - 1].len = 2;
				i++;
			}
		}
		i++;
	}
}

char	*line_surgery(char *line, t_token quote)
{
	const size_t	len_line = ft_strlen(line);
	char			*new_line;

	new_line = ft_calloc(len_line, sizeof(char));
	if (new_line == 0)
		return (0);
	ft_strlcat(new_line, line, quote.pos + 1);
	ft_strlcat(new_line, &line[quote.pos + 1], len_line);
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
			output->line = line_surgery(input->line, input->tokens[i]);
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

t_tokenized_line *expander(char *line, t_hash_table *env)
{
	void			*tokens;
	void			*tokens_output;
	const size_t	len_line = ft_strlen(line);
	const size_t	size = sizeof(t_tokenized_line) + sizeof(t_token) * len_line;

	tokens = malloc(size);
	if (tokens == 0)
		return (0);
	tokens_output = malloc(size);
	if (tokens_output == 0)
		return (free_1_return_null(tokens));
	tokenize_string(line, tokens);
	print_tokens(tokens);
	if (turn_quoted_tokens_to_word(tokens) == 1)
		return (free_2_return_null(tokens, tokens_output));
	print_tokens(tokens);
	dollar_alone_is_dead(tokens);
	print_tokens(tokens);
	expand_variables(tokens, tokens_output, env);
	free(tokens);
	return (tokens_output);
}

t_tokenized_line	*lexer(t_tokenized_line *input)
{
	void			*tokens;
	void			*tokens_output;
	const size_t	len_line = ft_strlen(input->line);
	const size_t	size = sizeof(t_tokenized_line) + sizeof(t_token) * len_line;

	tokens = malloc(size * 4);
	if (tokens == 0)
		return (0);
	tokens_output = malloc(size);
	if (tokens_output == 0)
		return (free_1_return_null(tokens));
	expand_token_list(input, tokens);
	print_tokens(tokens);
	turn_whitespaces_to_word(tokens);
	print_tokens(tokens);
	turn_dead_to_word(tokens);
	print_tokens(tokens);
	if (remove_quotes(tokens, tokens + size) == 1)
		return (free_2_return_null(tokens, tokens_output));
	fuse_words(tokens + size, tokens + size * 2);
	print_tokens(tokens + size * 2);
	remove_whitespaces(tokens + size * 2, tokens + size * 3);
	print_tokens(tokens + size * 3);
	fuse_chevrons(tokens + size * 3, tokens_output);
	free(tokens);
	return (tokens_output);
}
