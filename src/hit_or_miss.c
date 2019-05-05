/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_or_miss.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlavreni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 10:38:00 by mlavreni          #+#    #+#             */
/*   Updated: 2019/02/15 10:38:01 by mlavreni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/rtv1.h"

int				compute_quadratic(t_vec tmp, t_pair *res)
{
	double		q;

	q = sqrt(tmp[1] * tmp[1] - 4.0 * tmp[0] * tmp[2]);
	if (q < 0)
	{
		res->d1 = -1;
		res->d2 = -1;
		return (0);
	}
	res->d1 = (-tmp[1] - q) / (2.0 * tmp[0]);
	res->d2 = (-tmp[1] + q) / (2.0 * tmp[0]);
	return (1);
}

void			obj_hit(int *id, double *dist, t_ray ray, t_params *p)
{
	int		tmp_id;
	double	tmp_dist;
	t_pair	res;

	*id = -1;
	*dist = __DBL_MAX__;
	tmp_id = -1;
	while (++tmp_id < p->obj_amount)
	{

		// p->obj_arr[tmp_id].get_dist(&p->obj_arr[tmp_id].shape, ray, &res, p);
		p->obj_arr[tmp_id].get_dist(&p->obj_arr[tmp_id], ray, &res, p);
		if (res.d1 < *dist && res.d1 > 0.00001 && res.d1 < __DBL_MAX__
			&& !(slice_intersect(res.d1, &p->obj_arr[tmp_id], ray, p)))
		{
			*id = tmp_id;
			*dist = res.d1;
			p->tmp_i_d.id = tmp_id;
			p->tmp_i_d.dist = res.d1;
		}
		if (res.d2 < *dist && res.d2 > 0.00001 && res.d2 < __DBL_MAX__
			&& !(slice_intersect(res.d2, &p->obj_arr[tmp_id], ray, p)))
		{
			*id = tmp_id;
			*dist = res.d2;
			p->tmp_i_d.id = tmp_id;
			p->tmp_i_d.dist = res.d2;
		}
	}
}

void			shpere_hit(t_obj *obj, t_ray ray, t_pair *res, t_params *p)
{
	t_vec		co;
	t_vec		tmp;

	co = ray.ori - obj->center;
	tmp[0] = dot(ray.dir, ray.dir);
	tmp[1] = 2.0 * dot(ray.dir, co);
	tmp[2] = dot(co, co) - obj->radius * obj->radius;

	if (!(compute_quadratic(tmp, res)))
		return ;
}

void			cylinder_hit(t_obj *obj, t_ray ray, t_pair *res, t_params *p)
{
	t_vec	tmp;
	t_vec	oc;
	double	rd_cd;
	double	oc_v;

	oc = ray.ori - obj->center;
	rd_cd = dot(ray.dir, obj->dir);
	oc_v = dot(oc, obj->dir);
	tmp[0] = dot(ray.dir, ray.dir) - rd_cd * rd_cd;
	tmp[1] = 2 * (dot(ray.dir, oc) - rd_cd * oc_v);
	tmp[2] = dot(oc, oc) - oc_v * oc_v - obj->radius * obj->radius;

	if (!(compute_quadratic(tmp, res)))
		return ;

	res->d1 = height_edge(obj, ray, rd_cd, res->d1);
	res->d2 = height_edge(obj, ray, rd_cd, res->d2);
}

void			cone_hit(t_obj *obj, t_ray ray, t_pair *res, t_params *p)
{
	t_vec	oc;
	t_vec	tmp;
	double	rd_cd;
	double	huh;

	oc = ray.ori - obj->center;
	huh = 1.0 + pow2(obj->angle);
	rd_cd = dot(ray.dir, obj->dir);
	tmp[0] = dot(ray.dir, ray.dir) - huh * pow2(rd_cd);
	tmp[1] = 2 * (dot(ray.dir, oc) - huh * rd_cd * dot(oc, obj->dir));
	tmp[2] = dot(oc, oc) - huh * pow2(dot(oc, obj->dir));

	if (!(compute_quadratic(tmp, res)))
		return ;

	res->d1 = height_edge(obj, ray, rd_cd, res->d1);
	res->d2 = height_edge(obj, ray, rd_cd, res->d2);
}


void			plane_hit(t_obj *obj, t_ray ray, t_pair *res, t_params *p)
{
	double		tmp1;
	double		tmp2;

	tmp1 = -dot(ray.ori - obj->center, obj->dir);
	tmp2 = dot(ray.dir, obj->dir);
	if (tmp2 != 0.0)
	{
		res->d1 = tmp1 / tmp2;
		res->d2 = -1;
		if (obj->radius > 0.0 && length((to_vec(res->d1) * ray.dir + ray.ori) - obj->center)
			> obj->radius)
			res->d1 = -1;
		return ;
	}
	res->d1 = -1;
	res->d2 = -1;
}

void			parabl_hit(t_obj *obj, t_ray ray, t_pair *res, t_params *p)
{
	t_vec		tmp;
	t_vec		oc;
	double		oc_od;
	double		rd_cd;

	oc = ray.ori - obj->center;
	oc_od = dot(oc, obj->dir);
	rd_cd = dot(ray.dir, obj->dir);

	tmp[0] = dot(ray.dir, ray.dir) - pow2(rd_cd);
	tmp[1] = 2 * (dot(ray.dir, oc) - rd_cd * (oc_od + 2.0 * obj->k));
	tmp[2] = dot(oc, oc) - oc_od * (oc_od + 4.0 * obj->k);

	if (!(compute_quadratic(tmp, res)))
		return ;

	res->d1 = height_edge(obj, ray, rd_cd, res->d1);
	res->d2 = height_edge(obj, ray, rd_cd, res->d2);
}
