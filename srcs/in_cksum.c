/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_cksum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 20:07:34 by agrumbac          #+#    #+#             */
/*   Updated: 2018/11/27 18:21:54 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

uint16_t		in_cksum(__unused const void *buffer, __unused size_t size)
{
	return (42);// TODO actually compute smth
}
