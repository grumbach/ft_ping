/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:05:58 by agrumbac          #+#    #+#             */
/*   Updated: 2019/01/23 20:51:59 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>

# define FT_PING_TTL			64
# define FT_PING_ID				42
# define FT_PING_DELAY			1

/*
** packet structure :
**   [ IP header    ]
**   [ ICMP header  ]
**   [ ICMP payload ]
*/

# define IP_HDR_SIZE			20
# define ICMP_HDR_SIZE			8
# define ICMP_PAYLOAD_SIZE		56
# define PACKET_SIZE			(IP_HDR_SIZE + ICMP_HDR_SIZE + ICMP_PAYLOAD_SIZE)

# define __unused				__attribute__((unused))
# define __noreturn				__attribute__((noreturn))
# define __warn_unused_result	__attribute__((warn_unused_result))

/*
** Muhahahahahahaha >:D-
** Cheap ass tail recursion
*/

#ifdef __linux__
# define JMP_RECV_PONG			asm("jmp recv_pong+15");__builtin_unreachable();
#elif __APPLE__
# define JMP_RECV_PONG			asm("jmp _recv_pong+15");__builtin_unreachable();
#endif

/*
** Socket i/o
*/

int				init_socket(void);
void			send_echo_request(int icmp_sock, const struct sockaddr *dest, \
					char *packet);
void			receive_echo_reply(int icmp_sock, struct sockaddr_in sockaddr, \
					char *packet);

/*
** Packet creation
*/

void			gen_ip_header(void *packet, u_int32_t dest);
void			gen_icmp_msg(void *packet, int seq);
uint16_t		in_cksum(const void *buffer, size_t size);

/*
** Packet analysis
*/

void			check_reply(void *packet, uint16_t seq);
void			print_stats(void);

/*
** Debug
*/

void			print_ip_icmp_packet(void *packet);

/*
** Error announcing
*/

void			fatal(const char * const message);
void			warn(const char * const message);

#endif
