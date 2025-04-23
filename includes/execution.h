/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:55:15 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/23 16:24:12 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"

typedef enum e_position
{
	FIRST,
	MID,
	LAST
}	t_position;

//path
char	*get_cmd_path(t_cmd cmd, char **env);

//execution
void	create_child_and_exec_cmd(t_cmd_list *list, char **env);

//
void	echo(t_cmd cmd);

#endif
