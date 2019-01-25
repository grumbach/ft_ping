/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet_analysis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 20:23:03 by agrumbac          #+#    #+#             */
/*   Updated: 2019/01/25 07:51:06 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

/*
** get_rtt
**   - calculates Round Trip Time in ms
*/

static suseconds_t	get_time(void)
{
	struct timeval	curr_time;

	if (gettimeofday(&curr_time, NULL) == -1)
	{
		warn("failed getting time of day");
		return (0);
	}
	return (curr_time.tv_sec * 1000000 + curr_time.tv_usec);
}

static suseconds_t	get_rtt(struct timeval *send_time)
{
	suseconds_t		curr_time = get_time();

	return (curr_time - send_time->tv_sec * 1000000 - send_time->tv_usec);
}

/*
** stats management
**   - so that ping exits gracefully...
*/

static t_stats		g_stats;

# define RTT_MIN(old, sample)		(old < sample ? old : sample)
# define RTT_MAX(old, sample)		(old > sample ? old : sample)

static void			update_rtt_stats(suseconds_t rtt, uint16_t seq)
{
	// if first packet init all
	if (seq == 1)
	{
		g_stats.rtt_min = rtt;
		g_stats.rtt_max = rtt;
		g_stats.rtt_total = rtt;
		g_stats.rtt_sq_total += rtt * rtt;
	}
	else
	{
		g_stats.rtt_min = RTT_MIN(g_stats.rtt_min, rtt);
		g_stats.rtt_max = RTT_MAX(g_stats.rtt_max, rtt);
		g_stats.rtt_total += rtt;
		g_stats.rtt_sq_total += rtt * rtt;
	}
}

void				update_stats(void)
{
	g_stats.packets_sent++;
}

void				set_stats_timer(void)
{
	g_stats.start_time = get_time();
}

void				print_stats(void)
{
	printf("\n--- %s ping statistics ---\n", NULL);
	printf("%u packets transmitted, ", g_stats.packets_sent);
	printf("%u received, ", g_stats.packets_recvd);

	if (g_stats.nb_errors)
		printf("+%u errors, ", g_stats.nb_errors);

	float loss = 1.0f - g_stats.packets_recvd / (float)g_stats.packets_sent;
	printf("%d%% packet loss, ", (int)(loss * 100.0f));

	suseconds_t timediff = get_time() - g_stats.start_time;
	printf("time %ld.%03ldms\n", timediff / 1000l, timediff % 1000l);

	if (g_stats.packets_recvd > 0)
	{
		suseconds_t	mdev;
		mdev = g_stats.rtt_total / g_stats.packets_recvd;
		g_stats.rtt_sq_total /= g_stats.packets_recvd;
		mdev = sqrtl(g_stats.rtt_sq_total - mdev * mdev);

		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
			g_stats.rtt_min / 1000.0f,
			g_stats.rtt_total / (float) g_stats.packets_recvd / 1000.0f,
			g_stats.rtt_max / 1000.0f,
			mdev / 1000.0f);
	}
}

/*
** icmp_responses
**   - will be NULL if response type is invalid
*/

#ifdef __linux__

static const char	*icmp_responses[] =
{
	[ICMP_DEST_UNREACH]		= "Destination Unreachable",
	[ICMP_SOURCE_QUENCH]	= "Source Quench",
	[ICMP_REDIRECT]			= "Redirect (change route)",
	[ICMP_ECHO]				= "Echo Request",
	[ICMP_TIME_EXCEEDED]	= "Time Exceeded",
	[ICMP_PARAMETERPROB]	= "Parameter Problem",
	[ICMP_TIMESTAMP]		= "Timestamp Request",
	[ICMP_TIMESTAMPREPLY]	= "Timestamp Reply",
	[ICMP_INFO_REQUEST]		= "Information Request",
	[ICMP_INFO_REPLY]		= "Information Reply",
	[ICMP_ADDRESS]			= "Address Mask Request",
	[ICMP_ADDRESSREPLY]		= "Address Mask Reply"
};

#elif __APPLE__

static const char	*icmp_responses[] =
{
	[ICMP_UNREACH]			= "Destination Unreachable",
	[ICMP_SOURCEQUENCH]		= "Source Quench",
	[ICMP_REDIRECT]			= "Redirect (change route)",
	[ICMP_ECHO]				= "Echo Request",
	[ICMP_TIMXCEED]			= "Time Exceeded",
	[ICMP_PARAMPROB]		= "Parameter Problem",
	[ICMP_TSTAMP]			= "Timestamp Request",
	[ICMP_TSTAMPREPLY]		= "Timestamp Reply",
	[ICMP_IREQ]				= "Information Request",
	[ICMP_IREQREPLY]		= "Information Reply",
	[ICMP_MASKREQ]			= "Address Mask Request",
	[ICMP_MASKREPLY]		= "Address Mask Reply"
};

#endif

/*
** check_reply
**   - checks reply packet, prints ping logs accordingly
**   - return true if seq is valid
*/

#ifdef __linux__

bool		check_reply(void *packet, uint16_t seq)
{
	struct iphdr	*ip = packet;
	struct icmphdr	*icmp = packet + IP_HDR_SIZE;
	const char		*error_str;
	char			*sender = inet_ntoa((struct in_addr){.s_addr = ip->saddr});
	suseconds_t		rtt = get_rtt(packet + IP_HDR_SIZE + ICMP_HDR_SIZE + ALIGN_TIMESTAMP);
	u_int16_t		recvd_seq = ntohs(icmp->un.echo.sequence);

	if (icmp->type != ICMP_ECHOREPLY)
	{
		if (icmp->type < sizeof(icmp_responses))
			error_str = icmp_responses[icmp->type];
		else
			error_str = NULL;
		printf("From %s icmp_seq=%hu %s", sender, recvd_seq, error_str);

		g_stats.nb_errors++;
	}
	else
	{
		printf("%hu bytes from %s: icmp_seq=%hu ttl=%hhu time=%ld.%02ld ms\n", \
			(uint16_t)(ntohs(ip->tot_len) - IP_HDR_SIZE), \
			sender, recvd_seq, ip->ttl, rtt / 1000l, rtt % 1000l);

		update_rtt_stats(rtt, seq);
	}

	g_stats.packets_recvd++;

	return (seq == recvd_seq);
}

#elif __APPLE__

bool		check_reply(void *packet, uint16_t seq)
{
	struct ip		*ip = packet;
	struct icmp		*icmp = packet + IP_HDR_SIZE;
	const char		*error_str;
	char			*sender = inet_ntoa((struct in_addr){.s_addr = ip->ip_src.s_addr});
	suseconds_t		rtt = get_rtt(packet + IP_HDR_SIZE + ICMP_HDR_SIZE + ALIGN_TIMESTAMP);
	u_int16_t		recvd_seq = ntohs(icmp->icmp_seq);

	if (icmp->icmp_type != ICMP_ECHOREPLY)
	{
		if (icmp->icmp_type < sizeof(icmp_responses))
			error_str = icmp_responses[icmp->icmp_type];
		else
			error_str = NULL;
		printf("From %s icmp_seq=%hu %s", sender, recvd_seq, error_str);

		g_stats.nb_errors++;
	}
	else
	{
		printf("%hu bytes from %s: icmp_seq=%hu ttl=%hhu time=%ld.%02ld ms\n", \
			(uint16_t)(ntohs(ip->ip_len) - IP_HDR_SIZE), \
			sender, recvd_seq, ip->ip_ttl, rtt / 1000l, rtt % 1000l);

		update_rtt_stats(rtt, seq);
	}

	g_stats.packets_recvd++;

	return (seq == recvd_seq);
}

#endif
