/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:04:47 by agrumbac          #+#    #+#             */
/*   Updated: 2018/12/12 03:39:26 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static int	init_socket(void)
{
	int	icmp_sock;

	icmp_sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (icmp_sock < 0)
		perror("socket");
	ssize_t				ret;
	   ret = setsockopt(icmp_sock, IPPROTO_IP, IP_HDRINCL, &(int[1]){1}, sizeof(int));
	if (ret == -1)
	   perror("opt");
	return (icmp_sock);
}

static void	send_echo_request(int icmp_sock, struct sockaddr_in sockaddr, char *packet)
{
	ssize_t	ret;

	ret = sendto(icmp_sock, packet, IP_HDR_SIZE + ICMP_MSG_SIZE, 0, \
		(struct sockaddr *)&sockaddr, sizeof(sockaddr));
	if (ret == -1)
		perror("sendto");

	printf("sending request:\n");
	print_ip_icmp_packet(packet);
}

static void	receive_echo_reply(int icmp_sock, struct sockaddr_in sockaddr)
{
	ssize_t		ret;
	char		echo_packet[ICMP_MSG_SIZE];
	char		buffer[512];
	struct iovec	io =
	{
		.iov_base = echo_packet,
		.iov_len = sizeof(echo_packet)
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

	printf("recieved answer:\n");
	print_ip_icmp_packet(echo_packet);
}

int		main(int ac, char **av)
{
	if (ac != 2)
	{
		dprintf(2, "Bad usage:\nft_ping <address>\n");
		return (EXIT_FAILURE);
	}

	char			*address = av[1];
	int			icmp_sock = init_socket();
	struct sockaddr_in	sockaddr;
	char			packet[IP_HDR_SIZE + ICMP_MSG_SIZE];

	sockaddr.sin_family = PF_INET;
	sockaddr.sin_addr.s_addr = inet_addr(address);
	sockaddr.sin_port = htons(0);

	gen_ip_header(packet, inet_addr(address));
	gen_icmp_msg(packet + IP_HDR_SIZE, 0);

	send_echo_request(icmp_sock, sockaddr, packet);
	receive_echo_reply(icmp_sock, sockaddr);

	return (EXIT_SUCCESS);
}
