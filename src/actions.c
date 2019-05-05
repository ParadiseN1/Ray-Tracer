/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlavreni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 10:47:33 by mlavreni          #+#    #+#             */
/*   Updated: 2019/02/15 10:47:34 by mlavreni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/rtv1.h"

t_params	*camera_move(int dir, t_params *p)
{
	t_vec		v;

	v = (t_vec){0., 0., 0.5};
	rotate_xyz(&v, p);
	if (dir == 0)
		p->camera->ori -= v;
	else if (dir == 1)
		p->camera->ori += v;
	else
	{
		rotate_y(&v, M_PI / 2);
		v[1] = 0;
		if (dir == 2)
			p->camera->ori -= v;
		else if (dir == 3)
			p->camera->ori += v;
	}
	return (p);
}

t_params	*camera_down_up(int dir, t_params *p)
{
	t_vec	vec;

	vec = (t_vec){0, 0.5f, 0};
	if (dir)
		p->camera->ori += vec;
	else
		p->camera->ori -= vec;
	return (p);
}

t_params	*camera_dir_helper(int axis, int sign, t_params *p)
{
	if (sign)
		p->camera->dir[axis] += 0.1;
	else
		p->camera->dir[axis] -= 0.1;
	return (p);
}

void		camera_dir(SDL_Keycode key, t_params *p)
{
	if (key == SDLK_q)
		create_threads(camera_dir_helper(1, 0, p), p->anaglyph);
	else if (key == SDLK_e)
		create_threads(camera_dir_helper(1, 1, p), p->anaglyph);
	else if (key == SDLK_r)
		create_threads(camera_dir_helper(0, 0, p), p->anaglyph);
	else if (key == SDLK_f)
		create_threads(camera_dir_helper(0, 1, p), p->anaglyph);
	else if (key == SDLK_z)
		create_threads(camera_dir_helper(2, 0, p), p->anaglyph);
	else if (key == SDLK_c)
		create_threads(camera_dir_helper(2, 1, p), p->anaglyph);
	else if (key == SDLK_i)
	{
		p->anaglyph = p->anaglyph == 1 ? 0 : 1;
		create_threads(p, p->anaglyph);
	}
	else if (key == SDLK_u)
	{
		p->antialiasing = p->antialiasing == 1 ? 0 : 1;
		create_threads(p, p->anaglyph);
	}
	else if (key == SDLK_1)
	{
		p->black = p->black == 1 ? 0 : 1;
		create_threads(p, p->anaglyph);
	}
}

int			key_press(SDL_Keycode key, t_params *p)
{
	if (key == SDLK_ESCAPE)
		exit(1);
	else if (key == SDLK_w)
		create_threads(camera_move(0, p), p->anaglyph);
	else if (key == SDLK_s)
		create_threads(camera_move(1, p), p->anaglyph);
	else if (key == SDLK_a)
		create_threads(camera_move(3, p), p->anaglyph);
	else if (key == SDLK_d)
		create_threads(camera_move(2, p), p->anaglyph);
	else if (key == SDLK_t)
		create_threads(camera_down_up(1, p), p->anaglyph);
	else if (key == SDLK_g)
		create_threads(camera_down_up(0, p), p->anaglyph);
	else if (key == SDLK_p)
	{
		p->pixel_view = (p->pixel_view == 1) ? 10 : 1;
		create_threads(p, p->anaglyph);
	}
	else
		camera_dir(key, p);
	return (1);
}
