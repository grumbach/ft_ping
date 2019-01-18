/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 19:29:40 by agrumbac          #+#    #+#             */
/*   Updated: 2019/01/18 20:57:40 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

# define FT_PING_FATAL				"\033[31m[FATAL ERROR] \033[0m"
# define FT_PING_WARN				"\033[33m[WARNING] \033[0m"

__noreturn
void			fatal(const char * const message)
{
	dprintf(2, FT_PING_FATAL);
	perror(message);

	exit(EXIT_FAILURE);
}

void			warn(const char * const message)
{
	dprintf(2, FT_PING_WARN);
	perror(message);
}
