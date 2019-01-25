/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:04:47 by agrumbac          #+#    #+#             */
/*   Updated: 2019/01/25 11:25:56 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static t_ping	g_ping =
{
	.sequence = 0,
	.sock = 0,
	.verbose_mode = false,
	.dest_addr = NULL,
	.dest =
	{
		.sin_family = AF_INET,
		.sin_port = 0
	}
};

static void	send_ping(__unused int sig)
{
	char		sent_packet[PACKET_SIZE];

	g_ping.sequence++;

	gen_ip_header(sent_packet, g_ping.dest.sin_addr.s_addr);
	gen_icmp_msg(sent_packet + IP_HDR_SIZE, g_ping.sequence);

	send_echo_request(g_ping.sock, (const struct sockaddr *)&g_ping.dest, \
		sent_packet, g_ping.verbose_mode);

	alarm(FT_PING_DELAY);
}

__noreturn
static void	recv_pong(void)
{
	char		rcvd_packet[PACKET_SIZE];

	while (true)
	{
		receive_echo_reply(g_ping.sock, g_ping.dest, rcvd_packet, g_ping.verbose_mode);
		check_reply(rcvd_packet, g_ping.sequence);
	}
}

static void	signal_exit(__unused int sig)
{
	close(g_ping.sock);
	print_stats(g_ping.sequence, g_ping.dest_addr);
	exit(EXIT_SUCCESS);
}

static char	*parse_input(int ac, char **av)
{
	struct addrinfo			hints = {.ai_family = AF_INET};
	struct addrinfo			*res;

	// sloppy check for "-v" verbose flag
	if (ac > 2 && av[1][0] == '-' && av[1][1] == 'v' && av[1][2] == '\0')
	{
		g_ping.verbose_mode = true;
		av++;
	}

	// get dest_addr
	if (getaddrinfo(av[1], NULL, &hints, &res))
	{
		dprintf(2, "ping: failed to get address from %s\n", av[1]);
		return (NULL);
	}
	g_ping.dest.sin_addr.s_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
	freeaddrinfo(res);

	return (av[1]);
}

int			main(int ac, char **av)
{
	if (ac < 2 || (av[1][0] == '-' && av[1][1] == 'h'))
	{
		dprintf(2, "Usage: %s [-hv] destination\n", av[0]);
		return (EXIT_FAILURE);
	}
	if (signal(SIGALRM, &send_ping) == SIG_ERR
	|| signal(SIGINT, &signal_exit) == SIG_ERR)
		fatal("alarm failed");

	g_ping.dest_addr = parse_input(ac, av);
	if (g_ping.dest_addr == NULL)
		return (EXIT_FAILURE);

	g_ping.sock = init_socket();
	set_stats_timer();

	printf("PING %s %d(%d) bytes of data.\n", g_ping.dest_addr, ICMP_PAYLOAD_SIZE, PACKET_SIZE);

	send_ping(0);
	recv_pong();
	__builtin_unreachable();
}
