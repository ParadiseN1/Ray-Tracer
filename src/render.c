/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlavreni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 10:35:02 by mlavreni          #+#    #+#             */
/*   Updated: 2019/02/15 10:35:03 by mlavreni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/rtv1.h"

// void		apply_light_sources(t_id_dist light,
// 				t_vec normal, t_vec hit, t_params *p)
// {
// 	int			i;
// 	t_vec		light_dir;
// 	t_ray		shadow;
// 	t_id_dist	tmp;

// 	float		n_dot_l;

// 	i = -1;
// 	while (++i < p->light_amount)
// 	{
// 		light.dist = length(p->light_sources[i].ori - hit);
// 		light_dir = unit_vector(p->light_sources[i].ori - hit);
// 		shadow = (t_ray){hit, light_dir};
// 		obj_hit(&tmp.id, &tmp.dist, shadow, p);

// 		n_dot_l = dot(normal, light_dir);
// 		if (n_dot_l > 0)
// 		// if (light.dist < tmp.dist)
// 		{
// 			p->color += to_vec(1) * to_vec(n_dot_l) / to_vec(light.dist * light.dist);
// 				// * to_vec(max(0, dot(normal, light_dir)));				
// 				// * p->obj_arr[light.id].diffuse));
// 			// p->color += to_vec(get_specular(reflect(p->ray_dir, normal),
// 			// 	p->light_sources[i].ori, p->obj_arr[light.id].specular));
// 		}
// 	}
// }

Uint32			get_texture_color(SDL_Surface *texture, int x, int y)
{
	int		bpp;
	uint8_t *pixels;

	(x < 0) ? x = 0 : 0;
	bpp = texture->format->BytesPerPixel;
	pixels = (uint8_t *)texture->pixels + y * texture->pitch + x * bpp;
	if (bpp == 1)
		return (*pixels);
	if (bpp == 2)
		return (*(uint16_t *)pixels);
	if (bpp == 3)
	{
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return (pixels[0] << 16 | pixels[1] << 8 | pixels[2]);
		else
			return (pixels[0] | pixels[1] << 8 | pixels[2] << 16);
	}
	if (bpp == 4)
		return (*(uint32_t *)pixels);
	return (0);
}
//Calculating x and y coords on texture to map on sphere

t_color spherical_mapping(SDL_Surface *texture, t_vec hit, t_vec center)
{
	t_vec d;
	double u;
	double v;

	d = unit_vector(hit - center);
	u = 0.5 + (atan2(d[2], d[0]) / (PI * 2));
	v = 0.5 - (asin(d[1]) / PI);
	return (int_to_color(get_texture_color(texture, u * texture->w, v * texture->h)));
}

void		print_vec(t_vec vec)
{
	printf(" %f %f %f\n", vec[0], vec[1], vec[2]);
}

// change the name 
double		compute_light(t_vec hit, t_vec normal, int obj_id, t_params *p)
{
	int		i;
	double	light_coef;
	double	tmp;
	double	dot_product;
	char 	*sas;
	t_vec	light;
	t_ray	shadow;
	t_vec	specular_ray;
	t_id_dist	shadow_id_dist;
	t_id_dist	tmp_i_d;

	i = -1;
	light_coef = 0.0;
	tmp = 0.0;
	while (++i < p->light_amount)
	{	
		// sas = ft_strstr(p->light_sources[i].type, "ambient");
		if (p->light_sources[i].type == 1)
			light_coef += p->light_sources[i].intensity;
		else 
		{
			if (p->light_sources[i].type == 2)
				light = p->light_sources[i].ori - hit;
			else
				light = p->light_sources[i].dir;

			// *** Here is part for blAcK magic ***
			if (((dot(unit_vector(light), normal) < 0) || (dot(p->tmp_ray.dir, normal) > 0))
				 && ((dot(unit_vector(light), normal) > 0) || (dot(p->tmp_ray.dir, normal) < 0)))
				continue ;
				// break ;

			//shadows
			shadow = (t_ray){hit, unit_vector(light)};
			obj_hit(&tmp_i_d.id, &tmp_i_d.dist, shadow, p);
			if (length(p->light_sources[i].ori - hit) > tmp_i_d.dist)
				continue ;

			// diffuse
			dot_product = dot(normal, light);
			if (dot_product > 0)
				light_coef += p->light_sources[i].intensity * dot_product / (length(normal) * length(light));
			
			// pretty shity as I would say, 
			// specular
			if ((p->obj_arr[obj_id].specular >= 10) && (p->obj_arr[obj_id].specular <= 1000))
			{
				specular_ray =  to_vec(2 * dot_product) * normal - light;
				dot_product = dot(specular_ray, -p->tmp_ray.dir);
				if (dot_product > 0)
					light_coef += p->light_sources[i].intensity  * 
						powf(dot_product / (length(specular_ray) * length(-p->tmp_ray.dir)), p->obj_arr[obj_id].specular);
			}
		}
	}
	return ((light_coef > 1.0) ? 1.0 : light_coef);
}


t_color			ray_trace_2(t_vec ori, t_vec dir, int depth, t_params *p)
{
	t_ray	ray;
	t_id_dist	tmp;

	ray = (t_ray){ori, unit_vector(dir)};
	p->tmp_ray.ori = ori;
	p->tmp_ray.dir = ray.dir;
	obj_hit(&tmp.id, &tmp.dist, ray, p);
	if (tmp.id != -1)
		return (shading(tmp, point_at_parameter(tmp.dist, ray), depth, p));
	return (to_vec(0));

}

// good boy, now change the name of the fucntions and add reflaction_depth to parser with error check
t_color			shading(t_id_dist obj_id_dist, t_vec hit, int depth, t_params *p)
{
	double		ref;
	t_vec		normal;
	t_vec		ref_ray_dir;
	t_color		ref_color;
	t_color		color;

	ref = (p->obj_arr[obj_id_dist.id].reflection > 1.0) ?
		1.0 : p->obj_arr[obj_id_dist.id].reflection;
	color = p->obj_arr[obj_id_dist.id].color;
	if (p->obj_arr[obj_id_dist.id].texture != NULL)
		color = spherical_mapping(p->obj_arr[obj_id_dist.id].texture, hit, p->obj_arr[obj_id_dist.id].center);
	normal = p->obj_arr[obj_id_dist.id]
		.get_normal(&p->obj_arr[obj_id_dist.id], hit, p);

	if (length(p->obj_arr[obj_id_dist.id].shape.plane.normal) < 2
	&& length(p->obj_arr[obj_id_dist.id].shape.plane.normal) > 0.5){
		printf("ssss");
		color = to_vec(0);
	}
	color *= to_vec(compute_light(hit, normal, obj_id_dist.id, p));
	if ((depth < 1) || (ref <= 0.0))
		return (color);
	ref_ray_dir = reflect(-p->tmp_ray.dir, normal);
	ref_color = ray_trace_2(hit, ref_ray_dir, depth - 1, p);
	return (color * to_vec(1.0 - ref) + ref_color * to_vec(ref));
}

t_color			gray_scale(t_color color)
{
	color[0] = color[0] * 0.21 + color[1] * 0.72 + color[2] * 0.07;
	color[1] = color[0];
	color[2] = color[0];
	return (color);
}
t_color			ray_trace(float u, float v, t_params *p)
{
	t_ray		ray;
	t_vec		ray_dir;
	t_id_dist	tmp;
	t_color		l_clr;
	t_color		r_color;

	ray_dir = (t_vec)
	{
		(2.0 * u - 1.0) * p->fov * p->aspect_ratio,
		(1.0 - 2.0 * v) * p->fov,
		-1.0
	};
	rotate_xyz(&ray_dir, p);
	ray = (t_ray){p->camera->ori, unit_vector(ray_dir)};
	p->tmp_ray.ori = ray.ori;
	p->tmp_ray.dir = ray.dir;
	obj_hit(&tmp.id, &tmp.dist, ray, p);
	if (tmp.id != -1 && p->black == 1)
		return (gray_scale(shading(tmp, point_at_parameter(tmp.dist, ray),
			p->reflection_depth, p)));
	if (tmp.id != -1)
		return (shading(tmp, point_at_parameter(tmp.dist, ray),
			p->reflection_depth, p));
	return (to_vec(0));
}

// you may need to try new formula for cones
// some strange defects with parallel light in zenith
int			antialiasing(double u, double v,t_params *p, double x1, double y1)
{
	int i;
	int j;
	int	x;
	int	y;
	int n;
	t_color color;

	n = 3;
	i = 0;
	color = (t_color){0, 0, 0};
	x = x1;
	y = y1;
	if ((x <= n && x >= p->width - n) || (y <= n && y >= p->height - n)
									|| p->antialiasing == 0)
		return (get_color(ray_trace(u, v, p)));
	while (i < n)
	{
		j = 0;
		while (j < n)
		{
			u = ((float)x + (j - n / 2) + 0.5 + (double)(rand() % 101) / 100) / (float)p->width;
			v = ((float)y + (i - n / 2) + 0.5 + (double)(rand() % 101) / 100) / (float)p->height;
			color += ray_trace(u, v, p);
			j++;
		}
		i++;
	}
	color[0] = color[0] / pow(n, 2);
	color[1] = color[1] / pow(n, 2);
	color[2] = color[2] / pow(n, 2);
	return (get_color(color));
}
void		*render(void *thr)	
{
	int		x;
	int		y;
	float	u;
	float	v;
	int		tmp;
	int		i;
	int		j;
	t_thread	*t;
	t_params	*p;
	t = (t_thread*)thr;
	if ((p = (t_params*)malloc(sizeof(t_params))))
	{
		*p = *t->p;
		// ft_copy(t->p, p);
		// printf("%f\b", t->p->camera->dir[0]);
		// printf("%f\b", p->camera->dir[0]);
		// y = 0;
		// while (y < p->height)
		y = p->height / THREADS_AMOUNT * t->id;
		// printf("%d %d\n", y, p->height / THREADS_AMOUNT * (t->id + 1));
		while (y < p->height / THREADS_AMOUNT * (t->id + 1))
		{
			x = 0;
			while (x < p->width)
			{
				u = ((float)x + 0.5) / (float)p->width;
				v = ((float)y + 0.5) / (float)p->height;
				tmp = antialiasing(u, v, p, x, y);
				// tmp = get_color(ray_trace(u, v, p));
				// tmp = get_color(p->color) / (t->id + 1) + x / 5;
				i = -1;

				if (p->pixel_view != 1)
					while (++i < p->pixel_view)
					{
						j = -1;
						while (++j < p->pixel_view)
							img_put_pixel(p, x + j, y + i, tmp);
					}
				else
					img_put_pixel(p, x, y, tmp);
				x += p->pixel_view;
			}
			y += p->pixel_view;
		}
		free(p);
	}
	pthread_exit(0);
}

void			create_threads(t_params *p, int anaglyph)
{
	int			i;
	t_thread		threads[THREADS_AMOUNT];
	
	if (anaglyph == 1)
		anaglyph_render(p);
	else
	{
		i = -1;
		while (++i < THREADS_AMOUNT)
		{
			threads[i].id = i;
			threads[i].p = p;
			pthread_create(&(threads[i].thread_id),
				NULL, render, &(threads[i]));
		}
		i = -1;
		while (++i < THREADS_AMOUNT)
			pthread_join(threads[i].thread_id, NULL);
		if (p->anaglyph == 0)
			render_screen(p);
	}
	//render_text(p, "hello");
}
