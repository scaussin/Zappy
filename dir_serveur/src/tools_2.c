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
	printf(KYEL " Sending to sock %d: " KRESET, sock);
	replace_nl(str, len);
	printf("[%.*s]\n", len, str);
}

void	print_send_gfx(char *str)
{
	printf(KMAG " Sending to gfx : " KRESET);
	printf("[%.*s]\n", (int)strlen(str), str);
}

void	print_receive(int sock, char *str, int len)
{
	printf(KBLU " Receiving from sock %d: " KRESET, sock);
	replace_nl(str, len);
	printf("[%.*s] len: %d\n", len, str, len);
}
