/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur_types.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 17:11:32 by aleung-c          #+#    #+#             */
/*   Updated: 2017/03/07 17:11:34 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEUR_TYPES_H
# define SERVEUR_TYPES_H

typedef int								t_socket;
typedef struct sockaddr_in				t_sockaddr_in;
typedef struct sockaddr					t_sockaddr;
typedef struct in_addr					t_in_addr;

typedef struct s_serveur				t_serveur;
typedef struct s_client_entity			t_client_entity;

#endif
