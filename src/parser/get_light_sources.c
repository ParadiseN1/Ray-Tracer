/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_light_sources.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlavreni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/21 11:01:54 by mlavreni          #+#    #+#             */
/*   Updated: 2019/02/21 11:01:55 by mlavreni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void		set_default_light_sources(t_params *p)
{
	int		i;

	i = -1;
	while (++i < p->light_amount)
	{
		p->light_sources[i].ori = (t_vec){-422, -422, -422};
		p->light_sources[i].dir = (t_vec){-422, -422, -422};
		p->light_sources[i].intensity = 0.0; 
		p->light_sources[i].type = 1;
	}
}


int			light_type_checker(int type)
{
	// if (ft_strstr(line, "ambient")
	// 	|| ft_strstr(line, "point")
	// 	|| ft_strstr(line, "parallel"))
	if (type > 0 && type < 4)
		return (1);
	return (0);
}

int			get_light(char *line, int *amount, int *i, t_params *p)
{
	if (ft_strstr(line, "\t\t_origin: "))
		p->light_sources[*amount].ori = get_vec(line + 10);
	else if (ft_strstr(line, "\t\t_intensity: "))
		p->light_sources[*amount].intensity = fabs(ft_atof(line + 13));
	else if (ft_strstr(line, "\t\t_dir: "))
		p->light_sources[*amount].dir = get_vec(line + 7);
	else if (ft_strstr(line, "\t\t_type: ")
				&& light_type_checker(abs(ft_atoi(line + 8))))
		p->light_sources[*amount].type = abs(ft_atoi(line + 8));
	else if (ft_strncmp(line, "\0", 1) == 0)
		;
	else if (ft_strstr(line, "\tlight{"))
	{
		*amount += 1;
		*i = -1;
	}
	else
		return (0);
	return (1);
}

int			prepere_light(int fd, t_params *p)
{
	int			rat;
	char		*line;
	static int	amount = -1;
	int			i;
	int			tmp;

	i = 0;
	while ((rat = get_next_line(fd, &line)) > 0)
	{

		if (ft_strstr(line, "\t}") == line)
			return (cleaner(line,
				(i == 0) ? -2 : amount + 1));
		else if (ft_strncmp(line, "}", 1) == 0)
			return (cleaner(line, -1));
		else if (!(get_light(line, &amount, &i, p)))
			break ;
		free(line);
		i++;
		if (amount + 1 > p->light_amount)
			return (-2);
	}
	return (cleaner(line, -1));
}

int			get_light_sources(int fd, t_params *p)
{
	int		rat;
	int		res;
	char	*line;
	int		amount;
	t_light	*lights;

	while ((rat = get_next_line(fd, &line)) > 0)
	{
		if (ft_strncmp(line, "LIGHT_SOURCES", 13) == 0)
		{
			if (!(helper(&amount, 13, line)))
				break ;
			if (!(lights = (t_light*)malloc(sizeof(t_light) * amount)))
				return (0);
			p->light_sources = lights;
			p->light_amount = amount;
			set_default_light_sources(p);
			while ((amount = prepere_light(fd, p)) > 0)
				res = amount;
			if (res != p->light_amount || amount == -2)
				return (cleaner(line, 0));
			return (cleaner(line, 1));
		}
		free(line);
	}
	return (cleaner(line, 0));
}
