/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 08:53:58 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/17 16:26:40 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include <unistd.h>

static int	write_heredoc(char	*eof_signal, int write_end)
{
	char	*buffer;
	int		len_eof;

	len_eof = ft_strlen(eof_signal);
	while (1)
	{
		buffer = get_next_line(STDIN_FILENO);
		if (buffer == 0)
			return (-1);
		if (ft_strncmp(buffer, eof_signal, len_eof + 1) == 0)
			break ;
		if (write(write_end, buffer, ft_strlen(buffer)) == -1)
		{
			free(buffer);
			return (-1);
		}
		free(buffer);
	}
	free(buffer);
	return (0);
}

static int	create_heredoc(char **av)
{
	int		end[2];
	char	*eof_signal;

	if (pipe(end) == -1)
		return (-1);
	eof_signal = ft_strjoin(av[2], "\n");
	if (eof_signal == 0)
	{
		close(end[0]);
		close(end[1]);
		return (-1);
	}
	if (write_heredoc(eof_signal, end[1]) == -1)
	{
		close(end[1]);
		close(end[0]);
		free(eof_signal);
		return (-1);
	}
	free(eof_signal);
	close(end[1]);
	return (end[0]);
}

void	open_heredoc(int ac, char **av, char **envp)
{
	t_files	files;

	if (ac < 6)
		return ;
	files.infile = create_heredoc(av);
	if (files.infile == -1)
	{
		perror("here_doc");
		return ;
	}
	files.outfile = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (files.outfile == -1)
	{
		perror(av[ac - 1]);
		close(files.infile);
		return ;
	}
	pipex(files, ac - 4, &av[3], envp);
}
