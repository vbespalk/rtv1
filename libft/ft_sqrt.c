/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/05 19:04:29 by vbespalk          #+#    #+#             */
/*   Updated: 2018/02/05 19:04:31 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_sqrt(int nb)
{
	int num;

	num = 1;
	while ((num * num) <= nb)
	{
		if ((num * num) == nb)
		{
			return (num);
		}
		num++;
	}
	return (0);
}
