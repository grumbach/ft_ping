/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_packet.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:05:58 by agrumbac          #+#    #+#             */
/*   Updated: 2018/12/12 03:46:55 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	print_ip_icmp_packet(void *packet)
{
	struct iphdr *ip = packet;
	struct icmphdr *icmp = packet + IP_HDR_SIZE;

	printf("	ip->version [%x]\n"
		"	ip->ihl [%x]\n"
		"	ip->tos [%hhx]\n"
		"	ip->tot_len [%hx]\n"
		"	ip->id [%hx]\n"
		"	ip->frag_off [%hx]\n"
		"	ip->ttl [%hhx]\n"
		"	ip->protocol [%hhx]\n"
		"	ip->check [%hx]\n"
		"	ip->saddr [%x]\n"
		"	ip->daddr [%x]\n"
		"	icmp->type [%hhx]\n"
		"	icmp->code [%hhx]\n"
		"	icmp->un.echo.id [%x]\n"
		"	icmp->un.echo.sequence [%x]\n "
		"	icmp->checksum [%hx]\n ",
			ip->version,
			ip->ihl,
			ip->tos,
			ntohs(ip->tot_len),
			ntohs(ip->id),
			ntohs(ip->frag_off),
			ip->ttl,
			ip->protocol,
			ip->check,
			ip->saddr,
			ip->daddr,
			icmp->type,
			icmp->code,
			ntohs(icmp->un.echo.id),
			ntohs(icmp->un.echo.sequence),
			icmp->checksum
	);

}


