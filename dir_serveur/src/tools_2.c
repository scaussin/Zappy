/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 12:26:28 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/07 12:26:31 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

/*
**	Display functions for server prints.
*/

void	print_send(int sock, char *str, int len)
{
	replace_nl(str, len);
	if (len > MAX_PRINT_SIZE)
		printf(KYEL " Sending sock %d:%s [%.*s...] len: %d ", sock, KRESET,
			MAX_PRINT_SIZE, str, len);
	else
		printf(KYEL " Sending sock %d:%s [%.*s] len: %d ", sock, KRESET,
			len, str, len);
}

void	print_send_gfx(char *str)
{
	printf("Sending gfx: [%.80s]", str);
}

void	print_receive(int sock, char *str, int len)
{
	replace_nl(str, len);
	if (len > MAX_PRINT_SIZE)
		printf(KBLU "Receiving sock %d:%s [%.*s] len: %d ", sock, KRESET,
			MAX_PRINT_SIZE, str, len);
	else
		printf(KBLU " Receiving sock %d:%s [%.*s] len: %d ", sock, KRESET,
			len, str, len);
}
