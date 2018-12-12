/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_icmp_msg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 19:20:21 by agrumbac          #+#    #+#             */
/*   Updated: 2018/12/12 02:06:18 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#ifdef __linux__

static void	fill_icmp_header(struct icmp *ping, int id, int seq)
{
	ping->type = ICMP_ECHO;
	ping->code = 0;
	ping->echo.id = id;
	ping->echo.seq = htons(seq);
	ping->checksum = in_cksum(ping, ICMP_MSG_SIZE);
}

#elif __APPLE__

static void	fill_icmp_header(struct icmp *ping, int id, int seq)
{
	ping->icmp_type = ICMP_ECHO;
	ping->icmp_code = 0;
	ping->icmp_id = id;
	ping->icmp_seq = htons(seq);
	ping->icmp_cksum = in_cksum(ping, ICMP_MSG_SIZE);
}

#endif

static void	fill_random_data(void *buffer, size_t size)
{
	memset(buffer, 42, size);
}

void		gen_icmp_msg(void *packet, int seq)
{
	const size_t		hdr_size = sizeof(struct icmp);

	fill_random_data(packet + hdr_size, ICMP_MSG_SIZE - hdr_size);
	fill_icmp_header((struct icmp *)packet, 42, seq);
}
