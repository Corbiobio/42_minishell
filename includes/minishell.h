/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:32:50 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/27 16:41:15 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include <stddef.h>
# include "libft.h"

typedef struct s_cmd
{
	int		io[2];
	char	**cmd;
}	t_cmd;
//-1 in IO indicates error while opening file, -2 indicates no file provided

typedef struct s_cmd_list
{
	size_t	nb_cmd;
	t_cmd	cmds[];
}	t_cmd_list;

typedef enum e_error
{
	ERROR_UNKNOWN,
	ERROR_QUOTE_UNCLOSED,
	ERROR_REDIRECTION_NO_FILENAME
}	t_error;

void			free_cmd_list(t_cmd_list *list);
struct termios	set_signal_handler_main(void);
int				print_error_return_one(t_error error);
t_hash_table	*convert_env_to_table(char **env);

#endif //MINISHELL_H
