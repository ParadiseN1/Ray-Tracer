/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_obj_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlavreni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/21 10:59:26 by mlavreni          #+#    #+#             */
/*   Updated: 2019/02/21 10:59:28 by mlavreni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int		get_props(t_obj *obj, char *line)
{
	if (ft_strstr(line, "\t\t_angle: "))
		(*obj).angle = ft_atof(line + 9);
	else if (ft_strstr(line, "\t\t_radius: "))
		(*obj).radius = ft_atof(line + 10);
	else if (ft_strstr(line, "\t\t_color: "))
		(*obj).color = color_checker((t_color)get_vec(line + 9));
	else if (ft_strstr(line, "\t\t_specular: "))
		(*obj).specular = ft_atof(line + 12);
	else if (ft_strstr(line, "\t\t_reflection: "))
		(*obj).reflection = ft_atof(line + 14);
	else if (ft_strstr(line, "_texture: "))
	{
		if (((*obj).texture = SDL_LoadBMP(line + 12)) == NULL)
		{
			ft_putstr("Faled to load Texture!");
			exit (0);
		}
	}
	else if (ft_strstr(line, "\t\t_height: "))
		(*obj).height = ft_atoi(line + 10);
	else if (ft_strstr(line, "\t\t_height_up: "))
		(*obj).height_up = ft_atoi(line + 13);
	else if (ft_strstr(line, "\t\t_height_down: "))
		(*obj).height_down = ft_atoi(line + 15);
	else if (ft_strstr(line, "\t\t_slice_ori: "))
		(*obj).slice_ori = get_vec(line + 13);
	else if (ft_strstr(line, "\t\t_slice_dir: "))
		(*obj).slice_dir = unit_vector(get_vec(line + 13));
	else if (ft_strstr(line, "\t\t_scalar: "))
		(*obj).k = ft_atof(line + 10);
	else
		return (0);
	return (1);
}

int		add_obj(t_obj *obj, t_pokemon pok, int fd)
{
	int		rat;
	char	*line;
	int		i;

	i = 0;
	(*obj).get_dist = pok.get_dist;
	(*obj).get_normal = pok.get_noraml;
	(*obj).texture = NULL;
	while ((rat = get_next_line(fd, &line)) > 0)
	{
		if (ft_strstr(line, "\t}") == line)
			return (cleaner(line,
				(i == 0) ? 0 : 1));
		else if (ft_strstr(line, "\t\t_center: "))
			(*obj).center = get_vec(line + 10);
		else if (ft_strstr(line, "\t\t_dir: "))
			(*obj).dir = unit_vector(get_vec(line + 7));
		else if (!(get_props(obj, line)))
			break ;
		free(line);
		i++;
	}
	return (cleaner(line, 0));
}

int		add_sphere(t_obj *obj, t_pokemon pok, int fd)
{
	int		rat;
	char	*line;
	int		i;

	i = 0;
	(*obj).get_dist = pok.get_dist;
	(*obj).get_normal = pok.get_noraml;
	(*obj).texture = NULL;
	while ((rat = get_next_line(fd, &line)) > 0)
	{
		if (ft_strstr(line, "\t}") == line)
			return (cleaner(line, (i != 7) ? 0 : 1));
		if (ft_strstr(line, "_center: "))
			(*obj).shape.sphere.center = get_vec(line + 10);
		else if (ft_strstr(line, "_radius: "))
			(*obj).shape.sphere.radius = fabs(ft_atof(line + 10));
		else if (!(get_props(obj, line)))
			break ;
		free(line);
		i++;
	}
	return (cleaner(line, 0));
}



int		add_cylinder(t_obj *obj, t_pokemon pok, int fd)
{
	int		rat;
	char	*line;
	int		i;

	i = 0;
	(*obj).get_dist = pok.get_dist;
	(*obj).get_normal = pok.get_noraml;
	(*obj).texture = NULL;
	while ((rat = get_next_line(fd, &line)) > 0)
	{
		if (ft_strstr(line, "\t}") == line)
			return (cleaner(line,
				(i != 8) ? 0 : 1));
		else if (ft_strstr(line, "_center: "))
			(*obj).shape.cylinder.center = get_vec(line + 10);
		else if (ft_strstr(line, "_dir: "))
			(*obj).shape.cylinder.dir = get_vec(line + 7);
		else if (ft_strstr(line, "_radius: "))
			(*obj).shape.cylinder.radius = fabs(ft_atof(line + 10));
		else if (ft_strstr(line, "_height: "))
			(*obj).shape.cylinder.height = abs(ft_atoi(line + 10));
		else if (!(get_props(obj, line)))
			break ;
		free(line);
		i++;
	}
	return (cleaner(line, 0));
}

int		add_cone(t_obj *obj, t_pokemon pok, int fd)
{
	int		rat;
	char	*line;
	int		i;

	i = 0;
	(*obj).get_dist = pok.get_dist;
	(*obj).get_normal = pok.get_noraml;
	(*obj).texture = NULL;
	while ((rat = get_next_line(fd, &line)) > 0)
	{
		if (ft_strstr(line, "\t}") == line)
			return (cleaner(line,
				(i != 9) ? 0 : 1));
		else if (ft_strstr(line, "_center: "))
			(*obj).shape.cone.center = get_vec(line + 10);
		else if (ft_strstr(line, "_dir: "))
			(*obj).shape.cone.dir = get_vec(line + 7);
		else if (ft_strstr(line, "_angle: "))
			(*obj).shape.cone.angle = fabs(ft_atof(line + 9));
		else if (ft_strstr(line, "_height_up: "))
			(*obj).shape.cone.height_up = abs(ft_atoi(line + 13));
		else if (ft_strstr(line, "_height_down: "))
			(*obj).shape.cone.height_down = abs(ft_atoi(line + 15));
		else if (!(get_props(obj, line)))
			break ;
		free(line);
		i++;
	}
	return (cleaner(line, 0));
}

int		add_plane(t_obj *obj, t_pokemon pok, int fd)
{
	int		rat;
	char	*line;
	int		i;

	i = 0;
	(*obj).get_dist = pok.get_dist;
	(*obj).get_normal = pok.get_noraml;
	(*obj).texture = NULL;
	while ((rat = get_next_line(fd, &line)) > 0)
	{
		if (ft_strstr(line, "\t}") == line)
			return (cleaner(line,
				(i != 7) ? 0 : 1));
		else if (ft_strstr(line, "_center: "))
			(*obj).shape.plane.center = get_vec(line + 10);
		else if (ft_strstr(line, "_normal: "))
			(*obj).shape.plane.normal = get_vec(line + 10);
		else if (ft_strstr(line, "_radius: "))
			(*obj).shape.plane.radius = ft_atof(line + 10);
		else if (!(get_props(obj, line)))
			break ;
		free(line);
		i++;
	}
	return (cleaner(line, 0));
}
