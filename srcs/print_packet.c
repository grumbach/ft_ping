/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_packet.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:05:58 by agrumbac          #+#    #+#             */
/*   Updated: 2019/01/25 10:48:22 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static const char		*packet_format = \
"\e[32m IP HEADER\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" \
"\e[32m| IPv  %x | IHL %x |    TOS %hhx     |          Total Length    %hx   |\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" \
"\e[32m|         Identification    %04hx      |   Fragment Offset  %hx    |\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" \
"\e[32m|   TTL   %hhx    |   Protocol %hhx  |       Header Checksum %04hx    |\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" \
"\e[32m|                     Source Address    %08x                |\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" \
"\e[32m|                  Destination Address  %08x                |\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" \
"\e[32m ICMP HEADER\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" \
"\e[32m|    Type %hhx     |    Code %hhx     |        Checksum %04hx          |\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" \
"\e[32m|           Identifier %x       |        Sequence Number %x      |\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\e[0m\n";

#ifdef __linux__

void	print_ip_icmp_packet(void *packet)
{
	struct iphdr *ip = packet;
	struct icmphdr *icmp = packet + IP_HDR_SIZE;

	printf(packet_format, ip->version, ip->ihl, ip->tos, ntohs(ip->tot_len), \
		ntohs(ip->id), ntohs(ip->frag_off), ip->ttl, ip->protocol, ip->check, \
		ip->saddr, ip->daddr, icmp->type, icmp->code, \
		icmp->checksum, ntohs(icmp->un.echo.id), ntohs(icmp->un.echo.sequence));
}

#elif __APPLE__

void	print_ip_icmp_packet(void *packet)
{
	struct ip *ip = packet;
	struct icmp *icmp = packet + IP_HDR_SIZE;

	printf(packet_format, ip->ip_v, ip->ip_hl, ip->ip_tos, ntohs(ip->ip_len), \
		ntohs(ip->ip_id), ntohs(ip->ip_off), ip->ip_ttl, ip->ip_p, ip->ip_sum, \
		ip->ip_src.s_addr, ip->ip_dst.s_addr, icmp->icmp_type, icmp->icmp_code, \
		icmp->icmp_cksum, ntohs(icmp->icmp_id), ntohs(icmp->icmp_seq));
}

#endif
