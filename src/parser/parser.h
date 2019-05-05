/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlavreni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/21 11:42:29 by mlavreni          #+#    #+#             */
/*   Updated: 2019/02/21 11:42:30 by mlavreni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../../inc/rtv1.h"

typedef struct s_pokemon	t_pokemon;

typedef int			(*t_add_obj_func)(t_obj *obj, t_pokemon pok, int fd);

struct				s_pokemon
{
	char			*shape;
	t_dist_func		get_dist;
	t_normal_func	get_noraml;
};

int					cleaner(char *line, int ret);
t_vec				get_vec(char *str);
double				ft_atof(char *str);

/*
**	parser.c
*/
t_color				color_checker(t_color clr);
void				set_obj_arr(t_params *p);
void				set_params(t_params *p);
int					find_obj_arr(int fd, t_params *p);
int					check_the_file(char *file, t_params *p);
t_params			*parser(char *file);

/*
**	add_obj_funcs.c
*/
int					add_obj(t_obj *obj, t_pokemon pok, int fd);
int					add_sphere(t_obj *obj, t_pokemon pok, int fd);
int					add_cylinder(t_obj *obj, t_pokemon pok, int fd);
int					add_cone(t_obj *obj, t_pokemon pok, int fd);
int					add_plane(t_obj *obj, t_pokemon pok, int fd);

/*
**	get_light_sources.c
*/
int					get_light_sources(int fd, t_params *p);

/*
**	gimy_gimy.c
*/
t_vec				get_vec(char *line);
int					get_camera(char *line, t_params *p);
int					get_params(int fd, t_params *p);

/*
**	pokemons.c
*/
t_pokemon			*set_pokemons(void);
int					whos_that_pokemon(int fd, t_pokemon *poks, t_params *p);

/*
**	cleaners.c
*/
int					cleaner(char *line, int res);
t_params			*ultimate_cleaner(t_params *p, int flag);
int					helper(int *amount, int size, char *line);

#endif
