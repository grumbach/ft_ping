/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:04:47 by agrumbac          #+#    #+#             */
/*   Updated: 2019/01/18 20:58:07 by agrumbac         ###   ########.fr       */
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

__noreturn
static void	ping_loop(void)
{
	char		sent_packet[PACKET_SIZE];
	char		rcvd_packet[PACKET_SIZE];

	// generate packet
	gen_ip_header(sent_packet, g_dest.sin_addr.s_addr);
	gen_icmp_msg(sent_packet, g_icmp_seq);
	g_icmp_seq++;

	// send, set timeout, receive
	send_echo_request(g_sock, (struct sockaddr *)&g_dest, sent_packet);
	alarm(FT_PING_DELAY);
	receive_echo_reply(g_sock, g_dest);

	// check
	check_reply(sent_packet, rcvd_packet, g_icmp_seq);

	// cheap ass tail recursion
	asm("jmp _ping_loop");
	__builtin_unreachable();
}

/*
** signal management
*/

static void	signal_timeout(__unused int sig)
{
	printf("Request timeout for icmp_seq %hu\n", g_icmp_seq);
	ping_loop();
}

static void	signal_exit(__unused int sig)
{
	close(g_sock);
	// TODO print some stats?
	exit(EXIT_SUCCESS);
}

int			main(int ac, char **av)
{
	if (ac != 2)
	{
		dprintf(2, "Bad usage:\nft_ping <address>\n");
		return (EXIT_FAILURE);
	}
	if (signal(SIGALRM, &signal_timeout) == SIG_ERR
	|| signal(SIGINT, &signal_exit) == SIG_ERR)
		fatal("alarm failed");

	g_sock = init_socket();
	g_dest.sin_addr.s_addr = inet_addr(av[1]);

	ping_loop();

	__builtin_unreachable();
}
