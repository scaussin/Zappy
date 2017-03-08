/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 12:26:19 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/07 12:26:21 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

char	*get_cmd_trim(char *str)
{
	int		len;

	if ((len = get_len_cmd(str)) < 0)
	{
		return (NULL);
	}
	return (strndup(str, len));
}

int		modulo(int a, int b)
{
	int	ret;

	ret = a % b;
	return (ret < 0 ? ret + b : ret);
}

void	print_buff(t_buffer buff)
{
	printf("buff [%s]\n", buff.buff);
	printf("start: %d len: %d\n", buff.start, buff.len);
	fflush(stdout);
}

char	*str_concat_realloc1(char *str1, char *str2)
{
	char	*ret;
	int		len1;
	int		len2;
	int		i;

	len1 = strlen(str1);
	len2 = strlen(str2);
	ret = (char *)s_malloc(sizeof(char) * (len1 + len2 + 1));
	i = -1;
	while (++i < len1)
		ret[i] = str1[i];
	i = -1;
	while (++i < len2)
		ret[i + len1] = str2[i];
	ret[i + len1] = '\0';
	free(str1);
	return (ret);
}
