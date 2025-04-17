/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:25:03 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/17 10:56:12 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"
#include "../libft/libft.h"
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

void	print_tokens(t_tokenized_line *line)
{
	size_t	i;

	printf("line: %s\n nb_tokens: %zu\n", line->line, line->nb_token);
	i = 0;
	while (i < line->nb_token)
	{
		printf("type: %d, pos: %zu, len: %zu\n", (int)line->tokens[i].type, line->tokens[i].pos, line->tokens[i].len);
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

	output->line = input->line;
	//*output = (t_tokenized_line){.line = input->line};
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
	while (i + 1 < input->nb_token)
	{
		add_token(output, input->tokens[i]);
		if (input->tokens[i].type == TYPE_GREATER && input->tokens[i + 1].type == TYPE_GREATER)
		{
			output->tokens[output->nb_token - 1].type = TYPE_GREATER_GREATER;
			output->tokens[output->nb_token - 1].len = 2;
			i++;
		}
		if (input->tokens[i].type == TYPE_LESSER && input->tokens[i + 1].type == TYPE_LESSER)
		{
			output->tokens[output->nb_token - 1].type = TYPE_LESSER_LESSER;
			output->tokens[output->nb_token - 1].len = 2;
			i++;
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

void	remove_quotes(t_tokenized_line *input, t_tokenized_line *output)
{
	size_t	i;

	i = 0;
	*output = (t_tokenized_line){.line = input->line};
	while (i < input->nb_token)
	{
		if (input->tokens[i].type != TYPE_SINGLE_QUOTE && input->tokens[i].type != TYPE_DOUBLE_QUOTE)
			add_token(output, input->tokens[i]);
		i++;
	}
	print_tokens(output);
}

t_tokenized_line	*lexer(char *line)
{
	t_tokenized_line	*tokens;
	t_tokenized_line	*tokens_quotes_removed;
	const size_t		len_line = ft_strlen(line);
	const int			size = sizeof(t_tokenized_line) + sizeof(t_token) * len_line;

	tokens = malloc(size * 4);
	if (tokens == 0)
		return (0);
	tokens_quotes_removed = malloc(size);
	if (tokens_quotes_removed == 0)
		return (free(tokens), (void *)0);
	ft_bzero(tokens, size * 4);
	printf("%p, %p\n", tokens, tokens + size * 4);
	tokenize_string(line, tokens);
	turn_quoted_tokens_to_word(tokens);
	fuse_words(tokens, tokens + size);
	remove_whitespaces(tokens + size, tokens + size * 2);
	fuse_chevrons(tokens + size * 2, tokens + size * 3);
	mark_single_quote_words(tokens + size * 3);
	remove_quotes(tokens + size * 3, tokens_quotes_removed);
	free(tokens);
	return (tokens_quotes_removed);
}
