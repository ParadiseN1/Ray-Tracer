/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlavreni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 15:29:46 by mlavreni          #+#    #+#             */
/*   Updated: 2018/10/25 15:31:50 by mlavreni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *dest, const char *src, size_t nb)
{
	int		i;
	size_t	k;

	i = 0;
	k = 0;
	while (*(dest + i) != '\0')
		i++;
	while (*(src + k) != '\0' && k < nb)
	{
		*(dest + i + k) = *(src + k);
		k++;
	}
	*(dest + k + i) = '\0';
	return (dest);
}
