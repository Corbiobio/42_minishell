/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sflechel <sflechel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:04:17 by sflechel          #+#    #+#             */
/*   Updated: 2025/04/23 13:20:07 by sflechel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/execution.h"
#include "../../includes/parser.h"
#include <stdlib.h>
#include <sys/wait.h>

int	main(int ac, char **av, char **env)
{
	int			status;
	t_cmd_list	*list;

	list = parser("cat << 'hi  -e' < file1 >file2|ls -a > ooomfie");
	create_child_and_exec_cmd(list, env);
	while (wait(&status) > 0)
		;
	free(list);
	(void)ac;
	(void)av;
	return (status);
}
