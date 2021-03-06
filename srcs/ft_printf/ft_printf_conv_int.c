/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_conv_int.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nforay <nforay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/09 17:13:02 by nforay            #+#    #+#             */
/*   Updated: 2020/03/09 18:47:24 by nforay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static void	print_width_int(t_state_machine *m)
{
	if (m->flag & (PLUS + SPACE) && m->args.d >= 0)
		m->fwidth--;
	if (!(m->flag & MINUS))
		while (m->fwidth && (m->fwidth - 1 > m->preci))
		{
			ft_putchar_fd(m->flag & ZERO ? '0' : ' ', 1);
			m->len++;
			m->fwidth--;
		}
	if (m->fwidth - m->preci > 0 && !(m->flag & MINUS))
	{
		if (m->flag & ZERO)
			ft_putchar_fd('0', m->fd);
		else
			ft_putchar_fd((m->args.d < 0) ? '-' : ' ', m->fd);
		m->fwidth--;
		m->len++;
	}
	if (m->flag & MINUS && m->args.d < 0)
	{
		ft_putchar_fd('-', m->fd);
		m->fwidth--;
		m->len++;
	}
}

static void	print_width_int_empty(t_state_machine *m)
{
	if (m->flag & (PLUS + SPACE))
	{
		m->fwidth--;
		if (m->flag & MINUS)
		{
			ft_putchar_fd((m->flag & PLUS) ? '+' : ' ', m->fd);
			m->len++;
		}
	}
	while (m->fwidth > 0)
	{
		ft_putchar_fd(' ', 1);
		m->len++;
		m->fwidth--;
	}
	if (!(m->flag & MINUS) && m->flag & (PLUS + SPACE) && m->args.d >= 0)
	{
		ft_putchar_fd((m->flag & PLUS) ? '+' : ' ', m->fd);
		m->len++;
	}
}

static void	print_hash(t_state_machine *m)
{
	if (m->flag & ZERO)
	{
		if (m->flag & (PLUS + SPACE) && m->args.d >= 0)
		{
			ft_putchar_fd((m->flag & PLUS) ? '+' : ' ', m->fd);
			m->len++;
		}
		print_width_int(m);
	}
	else
	{
		print_width_int(m);
		if (m->flag & (PLUS + SPACE) && m->args.d >= 0)
		{
			ft_putchar_fd((m->flag & PLUS) ? '+' : ' ', m->fd);
			m->len++;
		}
	}
}

static void	print_conv_int_end(t_state_machine *m, int size)
{
	print_hash(m);
	ft_printf_nbr_int_fd(m->args.d, size, m);
	while (m->fwidth > 0)
	{
		ft_putchar_fd(' ', 1);
		m->len++;
		m->fwidth--;
	}
}

void		print_conv_int(t_state_machine *m)
{
	int	size;

	m->args.d = (int)va_arg(m->params, int);
	if (!(m->args.d) && m->flag & POINT && (!(m->preci) || m->preci == -1))
	{
		print_width_int_empty(m);
		return ;
	}
	size = get_intlen(m->args.d, 1);
	if (m->flag & ZERO && (m->flag & POINT || m->flag & MINUS))
		m->flag &= ~ZERO;
	m->preci = (m->preci < size ? size : m->preci);
	m->fwidth = (m->fwidth < m->preci ? m->preci : m->fwidth);
	if ((m->fwidth == m->preci) && m->args.d < 0)
		m->fwidth = m->preci + 1;
	if (m->args.d < 0 && m->flag & ZERO)
	{
		ft_putchar_fd('-', m->fd);
		m->len++;
		m->fwidth--;
	}
	print_conv_int_end(m, size);
}
