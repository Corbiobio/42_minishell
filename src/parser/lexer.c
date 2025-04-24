/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:25:03 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/24 13:22:09 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include "../../libft/libft.h"
#include <stddef.h>
#include <stdio.h>

int	ft_iswhitespace(char c)
{
	if (c == ' ')
		return (1);
	if (c >= 9 && c <= 13)
		return (1);
	return (0);
}
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
	if (type == TYPE_WORD_SINGLE_QUOTE)
		return ("word_single_quote");
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
	print_tokens(tokens);
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
	print_tokens(output);
}

void	turn_quoted_tokens_to_word(t_tokenized_line *line)
{
	size_t		i;
	t_type		quote_type;

	i = 0;
	while (i < line->nb_token)
	{
		if (line->tokens[i].type == TYPE_SINGLE_QUOTE || line->tokens[i].type == TYPE_DOUBLE_QUOTE)
		{
			quote_type = line->tokens[i].type;
			i++;
			while (line->tokens[i].type != quote_type && i < line->nb_token)
			{
				if (line->tokens[i].type != TYPE_WHITESPACE)
					line->tokens[i].type = TYPE_WORD;
				i++;
			}
			if (i >= line->nb_token)
				printf("ERROR_QUOTE");
			i++;
		}
		i++;
	}
	print_tokens(line);
}

void	turn_whitespaces_to_word(t_tokenized_line *line)
{
	size_t		i;
	t_type		quote_type;

	i = 0;
	while (i < line->nb_token)
	{
		if (line->tokens[i].type == TYPE_SINGLE_QUOTE || line->tokens[i].type == TYPE_DOUBLE_QUOTE)
		{
			quote_type = line->tokens[i].type;
			i++;
			while (line->tokens[i].type != quote_type && i < line->nb_token)
			{
				line->tokens[i].type = TYPE_WORD;
				i++;
			}
			if (i >= line->nb_token)
				printf("ERROR_QUOTE");
			i++;
		}
		i++;
	}
	print_tokens(line);
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
			while (i < input->nb_token && input->tokens[i].type == TYPE_WORD)
			{
				output->tokens[output->nb_token - 1].len++;
				i++;
			}
			if (i < input->nb_token)
				add_token(output, input->tokens[i]);
		}
		i++;
	}
	print_tokens(output);
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
	print_tokens(output);
}

void	mark_single_quote_words(t_tokenized_line *line)
{
	size_t	i;

	i = 0;
	while (i + 2 < line->nb_token)
	{
		if (line->tokens[i].type == TYPE_SINGLE_QUOTE)
		{
			if (line->tokens[i + 1].type == TYPE_WORD && line->tokens[i + 2].type == TYPE_SINGLE_QUOTE)
			{
				line->tokens[i + 1].type = TYPE_WORD_SINGLE_QUOTE;
				i += 2;
			}
		}
		i++;
	}
	print_tokens(line);
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

void	remove_quotes(t_tokenized_line *input, t_tokenized_line *output)
{
	size_t	i;

	i = 0;
	*output = (t_tokenized_line){.line = input->line};
	while (i < input->nb_token)
	{
		if (input->tokens[i].type == TYPE_SINGLE_QUOTE || input->tokens[i].type == TYPE_DOUBLE_QUOTE)
		{
			output->line = line_surgery(input->line, input->tokens[i]);
			free(input->line);
			input->line = output->line;
			correct_positions(input, 0, 1, i);
			i++;
			continue ;
		}
		add_token(output, input->tokens[i]);
		i++;
	}
	print_tokens(output);
}

t_tokenized_line	*lexer(char *line, t_hash_table *env)
{
	void			*tokens;
	void			*tokens_output;
	size_t	len_line = ft_strlen(line);
	size_t	size = sizeof(t_tokenized_line) + sizeof(t_token) * len_line;

	tokens = malloc(size * 2);
	if (tokens == 0)
		return (0);
	tokens_output = malloc(size);
	if (tokens_output == 0)
		return (free(tokens), (void *)0);
	tokenize_string(line, tokens);
	turn_quoted_tokens_to_word(tokens);
	line = expand_variables(tokens, tokens + size, env);
	free(tokens);
	len_line = ft_strlen(line);
	size = sizeof(t_tokenized_line) + sizeof(t_token) * len_line;
	tokens = malloc(size * 4);
	if (tokens == 0)
		return (0);
	tokenize_string(line, tokens);
	turn_whitespaces_to_word(tokens);
	remove_quotes(tokens, tokens + size);
	fuse_words(tokens + size, tokens + size * 2);
	remove_whitespaces(tokens + size * 2, tokens + size * 3);
	fuse_chevrons(tokens + size * 3, tokens_output);
	mark_single_quote_words(tokens_output);
	free(tokens);
	return (tokens_output);
}
