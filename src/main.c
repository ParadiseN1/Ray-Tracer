/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlavreni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 10:35:15 by mlavreni          #+#    #+#             */
/*   Updated: 2019/02/15 10:35:21 by mlavreni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/rtv1.h"


void		sdl_event(t_params *p, SDL_Event *ev)
{
	if (ev->type == SDL_KEYDOWN)
		key_press(ev->key.keysym.sym, p);
	else if (ev->type == SDL_QUIT)
	{
		SDL_FreeSurface(p->surface);
		SDL_DestroyWindow(p->win_ptr);
		SDL_Quit();
		exit(1);
	}
}

int				close_window(t_params *p)
{
	(void)p;
	exit(0);
	return (0);
}

void		init(t_params *p)
{
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
	{
		//IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG; 
		p->win_ptr = SDL_CreateWindow("Rtv1", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, p->width, p->height, 0);
		p->surface = SDL_GetWindowSurface(p->win_ptr);
		p->pxls = p->surface->pixels;
	}
	TTF_Init();
	p->font = TTF_OpenFont("./resources/font/OpenSans-Bold.ttf", 14);
	if (p->font == NULL)
	{
		ft_putstr("Invalid font!\n");
		exit(0);
	}
	p->anaglyph = 0;
	p->antialiasing = 0;
	p->black = 0;
}

int				main(int ac, char **av)
{
	t_params		*params;
	SDL_Event		ev;

	if (ac == 2)
	{
		if (!(params = parser(av[1])))
		{
			system("leaks RTv1");
			return (ft_putstr("\nThe file is invalid. Just like you.\n\n"));
		}
	}
	else
		return (ft_putstr("\nusage: ./RTv1 [scene_file.rtv1]\n\n"));
	params->color = (t_color){255, 0, 0};
	create_threads(params, params->anaglyph);
	while (1)
	{
		while( SDL_PollEvent( &ev ) > 0 )
			sdl_event(params, &ev);
	}
	return (1);
}
