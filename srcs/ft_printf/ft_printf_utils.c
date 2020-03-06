#include "main.h"
#include "libft.h"

int			intlen(int n, int len)
{
	if (n <= 0)
		n = -n;
	while (n)
	{
		len++;
		n /= 10;
	}
	return (len);
}

void	print_width_s_null(t_state_machine *m)
{
	int strlen;
	
	strlen = !(m->args.s) ? 6 : (int)ft_strlen(m->args.s);
	(void)strlen;
	if (!(m->flag & POINT))
		m->fwidth -= 6;
	else if ((m->flag & POINT) && m->preci > 5)
		m->fwidth -= 6;
	while (m->fwidth > 0 && m->fwidth-- )
	{
		ft_putchar_fd(' ', m->fd);
		m->len++;
	}
}

void	print_width(t_state_machine *m)
{
	int	strlen;

	if ((m->flag & S_CONV) && !(m->args.s))
		return (print_width_s_null(m));
	if ((m->flag & S_CONV))
		strlen = (int)ft_strlen(m->args.s);
	if (m->fwidth)
	{
		if ((m->flag & S_CONV) && (strlen <= m->fwidth))
			if (m->flag & POINT && m->preci < strlen)
				m->fwidth = (m->fwidth - m->preci);
			else
				m->fwidth = (m->fwidth - strlen);
		else if ((m->flag & S_CONV) && (strlen > m->fwidth))
			if (m->flag & POINT)
				m->fwidth = (m->fwidth - m->preci);
			else
				m->fwidth = 0;
		else if (m->flag & C_CONV)
			m->len += (m->fwidth - 1);
		else if (m->flag & PER_CONV)
			m->fwidth--; //--
		while (m->fwidth > 0 && m->fwidth--)
		{
			ft_putchar_fd(m->flag & ZERO && (m->flag & (X_CONV + XMAJ_CONV + PER_CONV)) && !(m->flag & MINUS) ? '0' : ' ', m->fd);
			m->len++;
		}
	}
}

void	print_perc(t_state_machine *m)
{
	if (!(m->flag & MINUS) && m->flag & PER_CONV)
	{
		print_width(m);
	}
	if (m->flag & PER_CONV)
	{
		ft_putchar_fd('%', 1);
		m->len++;
	}
	if (m->flag & MINUS && m->flag & PER_CONV)
	{
		print_width(m);
	}
}

void	print_conv(t_state_machine *m)
{
	if (!(m->flag & MINUS) && (m->flag & C_CONV))
		print_width(m);
	if (m->flag & C_CONV)
		print_conv_chr(m);
	else if (m->flag & S_CONV)
		print_conv_str(m);
	else if (m->flag & P_CONV)
		print_conv_ptr(m);
	else if (m->flag & (D_CONV | I_CONV))
		print_conv_int(m);
	else if (m->flag & (U_CONV | X_CONV | XMAJ_CONV))
		print_conv_uns(m);
}

int		ft_conv_base_len(t_state_machine *m, int len, unsigned int nbr, char *base)
{
	if (nbr / ft_strlen(base) != 0)
		len = (ft_conv_base_len(m, len, (nbr / ft_strlen(base)), base));
	return (++len);
}

void	ft_putnbr_base(t_state_machine *m, unsigned int nbr, char *base)
{
	if (nbr / 16 != 0)
		ft_putnbr_base(m, (nbr / 16), base);
	write(1, base + (nbr % 16), 1);
	m->len++;
}

void	print_conv_str(t_state_machine *m)
{
	m->args.s = (char *)va_arg(m->params, char *);
	if (!(m->flag & MINUS) && (m->fwidth > 0))
		print_width(m);
	if (m->flag & POINT && m->args.s)
		while (m->preci > 0 && m->preci-- && *m->args.s)
		{
			ft_putchar_fd(*m->args.s++, 1);
			if (m->fwidth != 0)
				m->fwidth--;
			m->len++;
		}
	else
	{
		if (!(m->args.s) && !((m->flag & POINT) && m->preci < 6))
		{
			m->args.s = "(null)";
			m->len += 6;
		}
		else if (m->args.s)
			m->len += ft_strlen(m->args.s);
		else if (!(m->args.s) && !(m->flag & POINT))
			m->args.s = "";
		ft_putstr_fd(m->args.s, 1);
	}
	if (m->flag & MINUS)
		print_width(m);
}

void	print_conv_chr(t_state_machine *m)
{
	m->args.c = (int)va_arg(m->params, int);
	ft_putchar_fd(m->args.c, 1);
	m->len++;
}

void	print_width_uns(t_state_machine *m, int len)
{
	int	written;

	written = (m->preci >= len ? m->preci : len);
	ft_putnbr_fd(written, 2);
	while (m->fwidth > written)
	{
		ft_putchar_fd((m->flag & ZERO && !(m->flag & POINT)) ? '0' : ' ', 1);
		m->len++;
		written++;
	}
}

void	ft_putunsnbr_fd(t_state_machine *m, unsigned int n, int fd)
{
	if (n <= 9)
	{
		ft_putchar_fd(n + 48, fd);
		m->len++;
	}
	else
	{
		ft_putunsnbr_fd(m, n / 10, fd);
		ft_putchar_fd(n % 10 + 48, fd);
		m->len++;
	}
}

void	print_conv_uns(t_state_machine *m)
{
	int	strlen;

	strlen = 0;
	m->args.ux = (unsigned int)va_arg(m->params, unsigned int);
	if (!(m->args.ux) && m->preci == 0 && m->flag & POINT && m->fwidth == 0)
		return ;
	strlen = ft_conv_base_len(m, 0, m->args.ux, (m->flag & U_CONV) ? B_TEN : B_HEX);
	if (!(m->flag & MINUS))
		print_width_uns(m, strlen);
	if (m->flag & HASH)
	{
		ft_putstr_fd("0x", 1);
		m->len += 2;
	}
	if (m->flag & POINT && strlen < m->preci)
		while ((m->preci - strlen) > 0)
		{
			ft_putchar_fd('0', 1);
			strlen++;
			m->len++;
		}
	if (!(m->args.ux) && m->preci == 0 && m->flag & POINT)
	{
		ft_putchar_fd(' ', 1);
		m->len++;
	}
	else if (m->flag & X_CONV)
		ft_putnbr_base(m, m->args.ux, B_HEX);
	else if (m->flag & XMAJ_CONV)
		ft_putnbr_base(m, m->args.ux, B_MHEX);
	else if (m->flag & U_CONV)
		ft_putunsnbr_fd(m, m->args.ux, m->fd);
	if (m->flag & MINUS)
		print_width_uns(m, strlen);
}

void	extract_aste(t_state_machine *m)
{
	if (m->flag & POINT)
		m->preci = (int)va_arg(m->params, int);
	else
	{
		m->fwidth = (int)va_arg(m->params, int);
		if (m->fwidth < 0)
		{
			m->fwidth *= -1;
			m->flag |= MINUS;
		}
	}
	if (m->preci < 0)
		m->preci = 0;
	m->flag &= ~ASTER;
}
