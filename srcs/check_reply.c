/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_reply.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 20:23:03 by agrumbac          #+#    #+#             */
/*   Updated: 2019/01/18 20:35:10 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

// TODO code this

void		check_reply(char sent_packet[PACKET_SIZE], \
				char rcvd_packet[PACKET_SIZE], uint16_t seq)
{
	// OK
	if (sent_packet && rcvd_packet)
	{
		printf("64 bytes from 8.8.8.8: icmp_seq=%hu ttl=121 time=1.404 ms",
			seq);
	}
	// ERR
	else
	{
		printf("rand bytes from 8.8.8.8: icmp_seq=%hu ttl=121 time=1.404 ms",
			seq);
	}
}
