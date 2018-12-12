/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:04:47 by agrumbac          #+#    #+#             */
/*   Updated: 2018/12/12 02:36:52 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

/*  inet_v6

int					icmp_sock6;

icmp_sock6 = socket(AF_INET6, SOCK_DGRAM, IPPROTO_ICMPV6);
sockaddr.sin_family = PF_INET6;

*/

#ifdef THIS_IS_A_COMMENT
	struct msghdr
	{
		void         *msg_name;       /* optional address */
		socklen_t     msg_namelen;    /* size of address */
		struct iovec *msg_iov;        /* scatter/gather array */
		size_t        msg_iovlen;     /* # elements in msg_iov */
		void         *msg_control;    /* ancillary data, see below */
		size_t        msg_controllen; /* ancillary data buffer len */
		int           msg_flags;      /* flags on received message */
	};
#endif

int		main(int ac, char **av)
{
	if (ac != 2)
	{
		dprintf(2, "Bad usage:\nft_ping <address>\n");
		return (EXIT_FAILURE);
	}

	char				packet[IP_HDR_SIZE + ICMP_MSG_SIZE];
	int					icmp_sock;
	struct sockaddr_in	sockaddr;
	char				*address = av[1];

	icmp_sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_ICMP);

	sockaddr.sin_family = PF_INET;
	sockaddr.sin_addr.s_addr = inet_addr(address);
	sockaddr.sin_port = htons(0);

	gen_ip_header(packet, inet_addr(address));
	gen_icmp_msg(packet + IP_HDR_SIZE, 0);

	ssize_t ret = sendto(icmp_sock, &packet, sizeof(packet), 0, \
		(struct sockaddr *)&sockaddr, sizeof(sockaddr));

	if (ret == -1)
		perror("err");

	printf("[%zd]: %.*s\n", ret, ICMP_MSG_SIZE, packet);

/*
** TESTS from this line on
*/

	char				echo_packet[ICMP_MSG_SIZE];
	char				buffer[512];
	struct iovec		io =
	{
		.iov_base = echo_packet,
		.iov_len = sizeof(echo_packet)
	};
	struct msghdr		msg =
	{
		.msg_name = &sockaddr,
		.msg_namelen = sizeof(sockaddr),
		.msg_iov = &io,
		.msg_iovlen = 1,
		.msg_control = buffer,
		.msg_controllen = sizeof(buffer),
		.msg_flags = 0
	};

	ret = recvmsg(icmp_sock, &msg, 0);

	printf("[%zd]: %.*s\n", ret, ICMP_MSG_SIZE, echo_packet);

/*
** TESTS above this line
*/

	return (EXIT_SUCCESS);
}
