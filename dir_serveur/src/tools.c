/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 12:22:02 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/07 12:22:04 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

void	exit_error(char *error_log)
{
	perror(error_log);
	exit(errno);
}

/*
**	Safe mallocating.
*/

void	*s_malloc(size_t size)
{
	void	*ret;

	if (!(ret = malloc(size)))
		exit_error("malloc()");
	return (ret);
}

void	replace_nl(char *str, int len)
{
	int i;

	i = 0;
	while (i < len)
	{
		if (str[i] == '\n')
			str[i] = '*';
		i++;
	}
}

/*
**	A command finishes by '\n'. This function returns the length until that '\n'
*/

int		get_len_cmd(char *str)
{
	int		len;

	len = 0;
	while (*str && *str != '\n')
	{
		len++;
		str++;
	}
	if (!(*str))
		return (-1);
	return (len);
}
