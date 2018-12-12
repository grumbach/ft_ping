/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 18:05:58 by agrumbac          #+#    #+#             */
/*   Updated: 2018/12/12 02:36:20 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>

# define IP_HDR_SIZE			20
# define ICMP_MSG_SIZE			56

# define FT_PING_ID				42
# define FT_PING_TTL			64

# define __unused			__attribute__((unused))

uint16_t		in_cksum(const void *buffer, size_t size);

void			gen_ip_header(void *packet, u_int32_t dest);
void			gen_icmp_msg(void *packet, int seq);

#endif
