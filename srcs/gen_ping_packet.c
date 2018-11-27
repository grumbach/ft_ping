/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_ping_packet.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 19:20:21 by agrumbac          #+#    #+#             */
/*   Updated: 2018/11/27 22:33:45 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#ifdef __linux__

static void	fill_ping_header(struct icmp *ping, int id, int seq)
{
	ping->type = ICMP_ECHO;
	ping->code = 0;
	ping->echo.id = id;
	ping->echo.seq = htons(seq);
	ping->checksum = in_cksum(ping, PING_PACKET_SIZE);
}

#elif __APPLE__

static void	fill_ping_header(struct icmp *ping, int id, int seq)
{
	ping->icmp_type = ICMP_ECHO;
	ping->icmp_code = 0;
	ping->icmp_id = id;
	ping->icmp_seq = htons(seq);
	ping->icmp_cksum = in_cksum(ping, PING_PACKET_SIZE);
}

#endif

static void	fill_random_data(void *buffer, size_t size)
{
	ft_memset(buffer, 42, size);//actual random?
}

void		gen_ping_packet(char *packet, int seq)
{
	const size_t		hdr_size = sizeof(struct icmp);

	fill_random_data(packet + hdr_size, PING_PACKET_SIZE - hdr_size);
	fill_ping_header((struct icmp *)packet, 42, seq);
}
