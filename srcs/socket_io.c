/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_io.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 19:52:51 by agrumbac          #+#    #+#             */
/*   Updated: 2019/01/25 10:16:52 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

int		init_socket(void)
{
	int					icmp_sock;

	icmp_sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (icmp_sock < 0)
		fatal("failed opening ICMP socket");

	if (setsockopt(icmp_sock, IPPROTO_IP, IP_HDRINCL, \
		(int[1]){1}, sizeof(int)) == -1)
		fatal("failed to set socket option");

	return (icmp_sock);
}

void	send_echo_request(int icmp_sock, const struct sockaddr *dest, \
				char *packet, bool verbose_mode)
{
	ssize_t	ret;

	ret = sendto(icmp_sock, packet, PACKET_SIZE, 0, dest, sizeof(*dest));
	if (ret == -1)
		warn("sendto failed");

	if (verbose_mode)
	{
		printf("sending request:\n");
		print_ip_icmp_packet(packet);
	}
}

void	receive_echo_reply(int icmp_sock, struct sockaddr_in sockaddr, \
			char *packet, bool verbose_mode)
{
	char			buffer[512];
	ssize_t			ret;
	struct iovec	io =
	{
		.iov_base = packet,
		.iov_len = PACKET_SIZE
	};
	struct msghdr	msg =
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
	if (ret == -1)
		warn("recvmsg failed");

	if (verbose_mode)
	{
		printf("recieved answer:\n");
		print_ip_icmp_packet(packet);
	}
}
