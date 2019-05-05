/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaners.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlavreni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/21 11:00:45 by mlavreni          #+#    #+#             */
/*   Updated: 2019/02/21 11:00:46 by mlavreni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int			cleaner(char *line, int ret)
{
	free(line);
	return (ret);
}

t_params	*ultimate_cleaner(t_params *p, int flag)
{
	int		i;
	printf("%d\n", flag);
	free(p->camera);
	if (flag >= 1 && p->obj_arr != NULL)
		free(p->obj_arr);
	if (flag >= 2 && p->light_sources != NULL)
		free(p->light_sources);
	free(p);
	return (0);
}

int			helper(int *amount, int size, char *line)
{
	if (*(line + size) == '[')
	{
		*amount = ft_atoi(line + size + 1);
		if (*amount < 1 || *amount > 50)
			return (0);
	}
	else
		return (0);
	return (1);
}
