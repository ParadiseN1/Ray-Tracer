/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_light_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlavreni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 13:15:37 by mlavreni          #+#    #+#             */
/*   Updated: 2019/04/09 13:54:11 by mlavreni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int			prepere_light(int fd, t_params *p)
{
	int			rat;
	char		*line;
	static int	amount = -1;
	int			i;

	i = 0;
	while ((rat = get_next_line(fd, &line)) > 0)
	{
		if (ft_strstr(line, "\t}") == line)
			return (cleaner(line,
			(i != 3) ? -2 : amount + 1));
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





int		find_light_type(char *line, int fd, t_params *p)
{
	if (ft_strstr(line, "ambient"))
		add_ambient(fd, p);
}

// int		add_ambient(int fd, t_params *p)
// {
// 	int		rat;
// 	char	*line;
// 	int		i;

// 	i = 0;
// 	while ((rat = get_next_line(fd, &line)) > 0)
// 	{
// 		if (ft_strstr(line, "\t}") == line)
// 			return (cleaner(line, (i != 1) ? 0 : 1));
// 		if (ft_strstr(line, "_intensity: "))
// 			p->


// 		// else if (!(get_props(obj, line)))
// 		// 	break ;
// 		free(line);
// 		i++;
// 	}
// 	return (cleaner(line, 0));
// }
