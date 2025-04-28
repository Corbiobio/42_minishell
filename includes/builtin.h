/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:43:41 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/28 14:44:28 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

//echo
void	ft_echo(t_cmd cmd, int *status);

//export
void	ft_export(t_cmd cmd, t_hash_table *env, int *status);

#endif
