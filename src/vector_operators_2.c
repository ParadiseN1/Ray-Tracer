/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operators_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlavreni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 10:31:19 by mlavreni          #+#    #+#             */
/*   Updated: 2019/02/15 10:31:21 by mlavreni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/rtv1.h"

t_vec	to_vec(double val)
{
	return ((t_vec){val, val, val});
}

double		height_edge(t_obj *obj, t_ray ray, double rd_cd, double tmp_res)
{
	double	m;

	m = rd_cd * tmp_res + dot(ray.ori - obj->center, obj->dir);
	if (obj->height > 0)
		if (m < obj->height_down || m > obj->height)
			return (-1);
	return (tmp_res);
}

int			check_vec(t_vec v)
{
	int		i;

	i = -1;
	while (++i < 3)
	{
		if (v[i] == -422.0)
			return (0);
	}
	return (1);
}

int			slice_intersect(double dist, t_obj *obj, t_ray ray, t_params *p)
{
	t_vec	tmp;

	if (!(check_vec(obj->slice_ori) || !(check_vec(obj->slice_dir))))
		return (0);
	tmp = (ray.ori + to_vec(dist) * ray.dir) - obj->slice_ori;
	if (dot(tmp, obj->slice_dir) / (length(obj->slice_dir) * length(tmp)) <= 0.0)
		return (1);
	return (0);
}