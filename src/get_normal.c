/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_normal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlavreni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 10:47:11 by mlavreni          #+#    #+#             */
/*   Updated: 2019/02/15 10:47:12 by mlavreni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/rtv1.h"

t_vec		sphere_normal(t_obj *obj, t_vec hit, t_params *p)
{
	// return ((hit - shape->sphere.center) / to_vec(shape->sphere.radius));

	return (unit_vector(hit - obj->center));
}

t_vec		cylinder_normal(t_obj *obj, t_vec hit, t_params *p)
{
	t_vec	diff;
	t_vec	tmp;

	// old
	// diff = shape->cylinder.dir - shape->cylinder.center;
	// tmp = hit - shape->cylinder.center;
	// return (cross(diff, cross(tmp, diff)));

	// new 
	double	m;

	m = dot(p->tmp_ray.dir, obj->dir) * p->tmp_i_d.dist
		+ dot(p->tmp_ray.ori - obj->center, obj->dir);

	// return (unit_vector(hit - shape->cylinder.center - 
	// 	shape->cylinder.dir * to_vec(m)));	
	obj->normal = unit_vector(hit - obj->center -
		obj->dir * to_vec(m));
	return (obj->normal);

	// very new
	// return (hit - shape->cylinder.dir * to_vec(shape->cylinder.m));
}

t_vec		cone_normal(t_obj *obj, t_vec hit, t_params *p)
{
	// hit = cross(hit - shape->cone.center, shape->cone.dir);
	// hit = cross(hit, shape->cone.dir);
	// return (unit_vector(-hit));

	double	m;

	m = dot(p->tmp_ray.dir, obj->dir) * p->tmp_i_d.dist
		+ dot(p->tmp_ray.ori - obj->center, obj->dir);
	
	return (unit_vector(hit - obj->center
		- to_vec((1.0 + pow2(obj->angle)) * m) * obj->dir));

}

t_vec		plane_normal(t_obj *obj, t_vec hit, t_params *p)
{
// 	(void)hit;
	// return (unit_vector(shape->plane.normal));

	obj->dir = unit_vector(obj->dir);
	if (dot(p->tmp_ray.dir, obj->dir) < 0.0)
		return (obj->dir);
	else
		return (-obj->dir);
		// return (obj->dir * to_vec(-1));	
}

t_vec		parabl_normal(t_obj *obj, t_vec hit, t_params *p)
{
	double	m;

	m = dot(p->tmp_ray.dir, obj->dir) * p->tmp_i_d.dist
		+ dot(p->tmp_ray.ori - obj->center, obj->dir);
	return (unit_vector(hit - obj->center - obj->dir * to_vec(m * obj->k)));
}