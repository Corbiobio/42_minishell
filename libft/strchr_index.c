/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strchr_index.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:29:00 by edarnand          #+#    #+#             */
/*   Updated: 2025/04/30 16:55:38 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>

int	strchr_index(const char *s, int c)
{
	int	i;

	if (s == NULL)
		return (-1);
	i = 0;
	while (s[i] != '\0' && s[i] != (char)c)
		i++;
	if (s[i] == (char)c)
		return (i);
	return (-1);
}

int	strchr_last_index(const char *s, int c)
{
	int	i;
	int	len;

	if (s == NULL)
		return (-1);
	len = ft_strlen(s);
	i = len - 1;
	while (i >= 0 && s[i] != 'c')
	{
		if (s[i] == (char)c)
			return (i);
		i--;
	}
	return (-1);
}