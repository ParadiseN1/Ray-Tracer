/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlavreni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 10:29:03 by mlavreni          #+#    #+#             */
/*   Updated: 2019/02/15 10:29:11 by mlavreni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# if __linux__
#  define A_KEY 97
#  define W_KEY 119
#  define S_KEY 115
#  define D_KEY 100
#  define Q_KEY 113
#  define E_KEY 101
#  define R_KEY 114
#  define F_KEY 102
#  define Z_KEY 122
#  define C_KEY 99
#  define T_KEY 116
#  define G_KEY 103
#  define ESC_KEY 65307
# else
#  define A_KEY 0
#  define W_KEY 13
#  define S_KEY 1
#  define D_KEY 2
#  define Q_KEY 12
#  define E_KEY 14
#  define R_KEY 15
#  define F_KEY 3
#  define Z_KEY 6
#  define C_KEY 8
#  define T_KEY 17
#  define G_KEY 5
#  define P_KEY 35
#  define ESC_KEY 53
#  define PI 3.14
#  define RED 1
#  define BLUE 2
# endif

# define THREADS_AMOUNT 8

# include <stdlib.h>
# include <math.h>
# include "../libft/libft.h"
# include <time.h>
# include "mlx.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <pthread.h>
# include "../Frameworks/SDL2.framework/Headers/SDL.h"
# include "../Frameworks/SDL2_image.framework/Headers/SDL_image.h"
# include "../Frameworks/SDL2_ttf.framework/Headers/SDL_ttf.h"

/*
**	do you belive in magic? because I am 	(O w O )
*/
typedef double		t_dbl4 __attribute__((vector_size(sizeof(double)*4)));

typedef	t_dbl4		t_vec;

typedef t_dbl4		t_color;

// typedef float		t_float2 __attribute__((vector_size(sizeof(float)*2)));

// typedef t_float2	t_pair;

/*
**	go full on crutch. LETS GO!!!
*/
typedef struct		s_id_dist
{
	int				id;
	double			dist;
}					t_id_dist;

/*
**	important typedefs
*/

typedef struct		s_pair
{
	double			d1;
	double			d2;
}					t_pair;


typedef struct s_params		t_params;

typedef struct s_obj		t_obj;

typedef struct		s_ray
{
	t_vec			ori;
	t_vec			dir;
}					t_ray;

union				u_shape
{
	struct			s_sphere
	{
		t_vec		center;
		float		radius;
	}				sphere;
	struct			s_cylinder
	{
		t_vec		center;
		t_vec		dir;
		t_vec		normal;
		float		radius;
		int			height;
	}				cylinder;
	struct			s_cone
	{
		t_vec		center;
		t_vec		dir;
		float		angle;
		int			height_up;
		int			height_down;
	}				cone;
	struct			s_plane
	{
		t_vec		center;
		t_vec		normal;
		float		radius;
	}				plane;
};

typedef void		(*t_dist_func)(t_obj *obj, t_ray ray, t_pair *res, t_params *p);

typedef t_vec		(*t_normal_func)(t_obj *obj, t_vec hit, t_params *p);

typedef struct		s_obj
{
	int				is_negative;
	t_vec			center;
	t_vec			dir;
	t_vec			normal;
	float			angle;
	float			radius;
	int				height;
	int				height_up;
	int				height_down;

	t_dist_func		get_dist;
	t_normal_func	get_normal;
	union u_shape	shape;
	t_color			color;
	float			diffuse;
	float			specular;
	float			reflection;
	float			k;
	SDL_Surface		*texture;
	
	t_vec			slice_ori;
	t_vec			slice_dir;
}					t_obj;

typedef struct		s_image
{
	void			*image;
	int				*ptr;
	int				bpp;
	int				stride;
	int				endian;
}					t_image;

typedef struct		s_camera
{
	t_vec			ori;
	t_vec			dir;
}					t_camera;

typedef struct		s_light
{
	t_vec			ori;
	t_vec			dir;
	float			intensity;
	int				type;
}					t_light;

typedef struct		s_thread
{
	int				id;
	pthread_t		thread_id;
	t_params		*p;
}					t_thread;

typedef struct		s_params
{
	SDL_Window		*win_ptr;
	SDL_Surface		*surface;
	t_camera		*camera;
	t_light			*light_sources;
	t_obj			*obj_arr;
	int				width;
	int				height;
	int				obj_amount;
	int				light_amount;
	float			aspect_ratio;
	float			fov;
	t_ray			tmp_ray;
	t_color			color;
	t_color			env_light;
	char			*light_type;
	t_vec			tmp_light;
	t_id_dist		tmp_i_d;
	double			m;
	int				pixel_view;
	int				reflection_depth;
	int				complex_flag;
	t_vec			complex_ori;
	unsigned int 	*pxls;
	int				sign;
	TTF_Font		*font;
	Uint8			anaglyph;
	Uint8			antialiasing;
	Uint8			black;
}					t_params;

/*
**	typedef for parser
*/
t_params			*parser(char *line);
void				init(t_params *p);

/*
**	main.c
*/
t_image				*create_image(void *mlx_ptr, t_params *p);
int					close_window(t_params *p);

/*
**	secondary.c
*/
t_vec				point_at_parameter(double t, t_ray ray);
double				pow2(double a);
float				max(float a, float b);
int					get_color(t_color color);
t_color				int_to_color(int color);
int					img_put_pixel(t_params *p, int x, int y, int color);

/*
** get_normal.c
*/
t_vec				sphere_normal(t_obj *obj, t_vec hit, t_params *p);
t_vec				cylinder_normal(t_obj *obj, t_vec hit, t_params *p);
t_vec				cone_normal(t_obj *obj, t_vec hit, t_params *p);
t_vec				plane_normal(t_obj *obj, t_vec hit, t_params *p);
t_vec				parabl_normal(t_obj *obj, t_vec hit, t_params *p);

/*
**	hit_or_miss.c
*/
void				obj_hit(int	*id, double	*dist, t_ray ray, t_params *p);
void				shpere_hit(t_obj *obj, t_ray ray, t_pair *res, t_params *p);
void				cylinder_hit(t_obj *obj, t_ray ray, t_pair *res, t_params *p);
void				cone_hit(t_obj *obj, t_ray ray, t_pair *res, t_params *p);
void				plane_hit(t_obj *obj, t_ray ray, t_pair *res, t_params *p);
void				parabl_hit(t_obj *obj, t_ray ray, t_pair *res, t_params *p);
void				torus_hit(t_obj *obj, t_ray ray, t_pair *res, t_params *p);

/*
**	render.c
*/
float				get_specular(t_vec dir, t_vec light_dir, float specular);
void				apply_light_sources(t_id_dist light,
						t_vec normal, t_vec hit, t_params *p);
t_color				shading(t_id_dist light, t_vec hit, int depth, t_params *p);
t_color				ray_trace(float u, float v, t_params *p);
void				*render(void *thr);
void				create_threads(t_params *p, int anaglyph);

/*
**	actions.c
*/
t_params			*camera_move(int dir, t_params *p);
t_params			*camera_down_up(int dir, t_params *p);
t_params			*camera_dir_helper(int axis, int sign, t_params *p);
void				camera_dir(SDL_Keycode key, t_params *p);
int					key_press(SDL_Keycode key, t_params *p);

/*
** rotate.c
*/
void				rotate_x(t_vec *v, float angle);
void				rotate_y(t_vec *v, float angle);
void				rotate_z(t_vec *v, float angle);
void				rotate_xyz(t_vec *v, t_params *p);

/*
** vector_operators_1.c
*/
double				dot(t_vec a, t_vec b);
double				length(t_vec v);
t_vec				cross(t_vec a, t_vec b);
t_vec				unit_vector(t_vec v);
t_vec				reflect(t_vec ray_dir, t_vec normal);

/*
**	vector_operators_2.c
*/
t_vec				to_vec(double val);
// double				limit_cyl(t_obj *obj, t_ray ray, double rd_cd, double tmp_res);
// double				limit_cone(t_obj *obj, t_ray ray, double rd_cd, double tmp_res);
double				height_edge(t_obj *obj, t_ray ray, double rd_cd, double tmp_res);
int					check_vec(t_vec v);
int					slice_intersect(double t, t_obj *obj, t_ray ray, t_params *p);

/*
**	sdl_funcs.c
*/
void		render_text(t_params *p, char *str, int x, int y);
void        render_screen(t_params *p);
/*
**	anaglyph.c
*/
void		anaglyph_render(t_params *p);
void		black_n_white(t_params *p);
t_color		gray_scale(t_color color);

#endif
