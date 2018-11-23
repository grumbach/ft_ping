/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:04:47 by agrumbac          #+#    #+#             */
/*   Updated: 2018/11/23 20:18:18 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#ifdef __linux__

void	gen_ping_packet(struct icmp *ping, int id, int seq)
{
	ft_bzero(ping, sizeof(*ping));

	ping->type = ICMP_ECHO;
	ping->code = 0;
	ping->echo.id = id;
	ping->echo.seq = seq;
	ping->checksum = in_cksum((struct mbuf *)ping, sizeof(*ping));
}

#elif __APPLE__

void	gen_ping_packet(struct icmp *ping, int id, int seq)
{
	ft_bzero(ping, sizeof(*ping));

	ping->icmp_type = ICMP_ECHO;
	ping->icmp_code = 0;
	ping->icmp_id = id;
	ping->icmp_seq = seq;
	ping->icmp_cksum = in_cksum((struct mbuf *)ping, sizeof(*ping));
}

#endif

int		main(int ac, char **av)
{
	if (ac != 3)
		return (EXIT_FAILURE);

	int					icmp_sock;
	struct sockaddr_in	sockaddr;
	struct icmp			ping;
	char				*address = av[1];
	int					port = ft_atoi(av[2]);

	icmp_sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_ICMP);

	sockaddr.sin_family = PF_INET;
	sockaddr.sin_addr.s_addr = inet_addr(address);
	sockaddr.sin_port = htons(port);

	gen_ping_packet(&ping, 42, 0x01);

	sendto(icmp_sock, &ping, sizeof(ping), 0, \
		(struct sockaddr *)&sockaddr, sizeof(sockaddr));

	return (EXIT_SUCCESS);
}
