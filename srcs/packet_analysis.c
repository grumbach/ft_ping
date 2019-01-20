/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet_analysis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 20:23:03 by agrumbac          #+#    #+#             */
/*   Updated: 2019/01/20 06:03:08 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static char	*g_addr = NULL;

void		print_stats(void)
{
	printf("\n--- %s ping statistics ---\n", g_addr);
	printf("? packets transmitted, ? received, ?%% packet loss, time ?ms\n");
	printf("rtt min/avg/max/mdev = ?/?/?/? ms\n");
}

#ifdef __linux__

void		check_reply(void *packet, uint16_t seq)
{
	struct iphdr *ip = packet;
	struct icmphdr *icmp = packet + IP_HDR_SIZE;

	g_addr = inet_ntoa((struct in_addr){.s_addr = ip->saddr});
	if (icmp->type == ICMP_ECHOREPLY && seq == icmp->un.echo.sequence)
	{
		printf("%hu bytes from %s: icmp_seq=%hu ttl=%hhu time=TODO ms\n",
		(uint16_t)(ntohs(ip->tot_len) - IP_HDR_SIZE),
		g_addr,
		ntohs(icmp->un.echo.sequence),
		ip->ttl);
	}
	else
	{
		printf("error %hhu from %s: icmp_seq=%hu ttl=%hhu time=TODO ms\n",
		icmp->type,
		g_addr,
		ntohs(icmp->un.echo.sequence),
		ip->ttl);
	}
}

#elif __APPLE__

void		check_reply(void *packet, uint16_t seq)
{
	struct ip *ip = packet;
	struct icmp *icmp = packet + IP_HDR_SIZE;
}

#endif
