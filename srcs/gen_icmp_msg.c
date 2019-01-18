/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_icmp_msg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 19:20:21 by agrumbac          #+#    #+#             */
/*   Updated: 2019/01/18 20:44:32 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#ifdef __linux__

static void	fill_icmp_header(struct icmphdr *ping, int id, int seq)
{
	ping->type = ICMP_ECHO;
	ping->code = 0;
	ping->un.echo.id = htons(id);
	ping->un.echo.sequence = htons(seq);
	ping->checksum = in_cksum(ping, ICMP_PAYLOAD_SIZE);
}

#elif __APPLE__

static void	fill_icmp_header(struct icmp *ping, int id, int seq)
{
	ping->icmp_type = ICMP_ECHO;
	ping->icmp_code = 0;
	ping->icmp_id = htons(id);
	ping->icmp_seq = htons(seq);
	ping->icmp_cksum = in_cksum(ping, ICMP_PAYLOAD_SIZE);
}

#endif

static void	fill_random_data(void *buffer, size_t size)
{
	memset(buffer, 42, size);
}

void		gen_icmp_msg(void *packet, int seq)
{
	fill_random_data(packet + ICMP_HDR_SIZE, ICMP_PAYLOAD_SIZE - ICMP_HDR_SIZE);
	fill_icmp_header(packet, 42, seq);
}
