/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_cksum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 20:07:34 by agrumbac          #+#    #+#             */
/*   Updated: 2018/12/12 01:39:28 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

//TODO fix checksum!

uint16_t		in_cksum(__unused const void *buffer, __unused size_t size)
{
	asm volatile (".intel_syntax;\n"
	"  xor rax, rax\n"
	"  cmp rsi, 0x1\n"
	"  jne _loop\n"
	"_odd_case:\n"
	"  xor dx, dx\n"
	"  mov dh , BYTE PTR [rdi]\n"
	"  sub rsi, 1\n"
	"  jmp _add_cksum\n"
	"_loop:\n"
	"  mov dx, WORD PTR [rdi]\n"
	"  add rdi, 2\n"
	"  sub rsi, 2\n"
	"_add_cksum:\n"
	"  adc ax, dx\n" //TODO is carry swiched on 16bit overflows?
	"  cmp rsi, 1\n"
	"  jg _loop\n"
	"  je _odd_case\n"
	"  leave\n"
	"  ret\n");

	__builtin_unreachable();
}
