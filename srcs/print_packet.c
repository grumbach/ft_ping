/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_packet.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:05:58 by agrumbac          #+#    #+#             */
/*   Updated: 2019/01/20 06:05:08 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#ifdef __linux__

void	print_ip_icmp_packet(void *packet)
{
	struct iphdr *ip = packet;
	struct icmphdr *icmp = packet + IP_HDR_SIZE;

	printf("\e[34m	ip->version [%x]"
		"	ip->ihl [%x]"
		"	ip->tos [%hhx]"
		"	ip->tot_len [%hx]\n"
		"	ip->id [%hx]"
		"	ip->frag_off [%hx]"
		"	ip->ttl [%hhx]"
		"	ip->protocol [%hhx]\n"
		"	ip->check [%hx]"
		"	ip->saddr [%x]"
		"	ip->daddr [%x]"
		"	icmp->type [%hhx]\n"
		"	icmp->code [%hhx]"
		"	icmp->un.echo.id [%x]"
		"	icmp->un.echo.sequence [%x]"
		"	icmp->checksum [%hx]\e[0m\n",
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

#elif __APPLE__

void	print_ip_icmp_packet(void *packet)
{
	struct ip *ip = packet;
	struct icmp *icmp = packet + IP_HDR_SIZE;

	printf("\e[34m	ip->version [%x]"
		"	ip->ihl [%x]"
		"	ip->tos [%hhx]"
		"	ip->tot_len [%hx]\n"
		"	ip->id [%hx]"
		"	ip->frag_off [%hx]"
		"	ip->ttl [%hhx]"
		"	ip->protocol [%hhx]\n"
		"	ip->check [%hx]"
		"	ip->saddr [%x]"
		"	ip->daddr [%x]"
		"	icmp->type [%hhx]\n"
		"	icmp->code [%hhx]"
		"	icmp->un.echo.id [%x]"
		"	icmp->un.echo.sequence [%x]"
		"	icmp->checksum [%hx]\e[0m\n",
			ip->ip_v,
			ip->ip_hl,
			ip->ip_tos,
			ntohs(ip->ip_len),
			ntohs(ip->ip_id),
			ntohs(ip->ip_off),
			ip->ip_ttl,
			ip->ip_p,
			ip->ip_sum,
			ip->ip_src.s_addr,
			ip->ip_dst.s_addr,
			icmp->icmp_type,
			icmp->icmp_code,
			ntohs(icmp->icmp_id),
			ntohs(icmp->icmp_seq),
			icmp->icmp_cksum
	);
}

#endif
