/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_ip_header.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 01:42:56 by agrumbac          #+#    #+#             */
/*   Updated: 2018/12/12 02:32:12 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#ifdef __linux__

static inline void	fill_ip_header(struct iphdr *ip, u_int32_t dest)
{
	ip->version = 4;
	ip->ihl = IP_HDR_SIZE / 4;
	ip->tos = 0;
	ip->tot_len = htons(IP_HDR_SIZE + ICMP_MSG_SIZE);
	ip->id = 0;
	ip->frag_off = 0;
	ip->ttl = FT_PING_TTL;
	ip->protocol = 1;
	ip->check = 0;
	ip->saddr = INADDR_ANY;
	ip->daddr = dest;
}

#elif __APPLE__

static inline void	fill_ip_header(struct ip *ip, u_int32_t dest)
{
	ip->ip_v = 4;
	ip->ip_hl = IP_HDR_SIZE / 4;
	ip->ip_tos = 0;
	ip->ip_len = htons(IP_HDR_SIZE + ICMP_MSG_SIZE);
	ip->ip_id = 0;
	ip->ip_off = 0;
	ip->ip_ttl = FT_PING_TTL;
	ip->ip_p = 1;
	ip->ip_sum = 0;
	ip->ip_src.s_addr = INADDR_ANY;
	ip->ip_dst.s_addr = dest;
}

#endif

/*
** gen_ip_header
**   provide dest address in network byte order
*/

void			gen_ip_header(void *packet, u_int32_t dest)
{
	fill_ip_header(packet, dest);
}