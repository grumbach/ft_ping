/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:04:47 by agrumbac          #+#    #+#             */
/*   Updated: 2019/01/20 06:32:51 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static uint16_t		g_icmp_seq = 0;
static int			g_sock;
struct sockaddr_in	g_dest =
{
	.sin_family = AF_INET,
	.sin_port = 0
};

static void	send_ping(int sig)
{
	char		sent_packet[PACKET_SIZE];

	// TODO timeout case
	if (sig)
		printf("Request timeout for icmp_seq %hu\n", g_icmp_seq++);

	gen_ip_header(sent_packet, g_dest.sin_addr.s_addr);
	gen_icmp_msg(sent_packet + IP_HDR_SIZE, g_icmp_seq);

	send_echo_request(g_sock, (const struct sockaddr *)&g_dest, sent_packet);

	alarm(FT_PING_DELAY);
}

__noreturn
static void	recv_pong(void)
{
	char		rcvd_packet[PACKET_SIZE];

	while (1)
	{
		receive_echo_reply(g_sock, g_dest, rcvd_packet);
		check_reply(rcvd_packet, g_icmp_seq);
	}
}

static void	signal_exit(__unused int sig)
{
	close(g_sock);
	print_stats();
	exit(EXIT_SUCCESS);
}

int			main(int ac, char **av)
{
	if (ac != 2)
	{
		dprintf(2, "Bad usage:\nft_ping <address>\n");
		return (EXIT_FAILURE);
	}
	if (signal(SIGALRM, &send_ping) == SIG_ERR
	|| signal(SIGINT, &signal_exit) == SIG_ERR)
		fatal("alarm failed");

	g_sock = init_socket();
	g_dest.sin_addr.s_addr = inet_addr(av[1]);

	printf("PING %s %d(%d) bytes of data.\n", av[1], ICMP_PAYLOAD_SIZE, PACKET_SIZE);

	send_ping(0);
	recv_pong();
	__builtin_unreachable();
}
