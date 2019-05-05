/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vprypesh <vprypesh@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 18:43:10 by vprypesh          #+#    #+#             */
/*   Updated: 2019/05/02 18:43:11 by vprypesh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/rtv1.h"

void        render_screen(t_params *p)
{
    render_text(p, "W, A, S, D - move", 20, 10);
    render_text(p, "Q, E - turn right / left", 20, 35);
    render_text(p, "R, F - look upside / down", 20, 60);
    render_text(p, "Z, C - rotate right / left", 20, 85);
	if (p->pixel_view > 1)
    	render_text(p, "P - Pixel mode (enabled)", 20, 110);
	else
    	render_text(p, "P - Pixel mode (disabled)", 20, 110);
	if (p->anaglyph == 1)
		render_text(p, "I - 3D mode (enabled)", 20, 135);
	else
		render_text(p, "I - 3D mode (disabled)", 20, 135);
	if (p->antialiasing == 1)
		render_text(p, "U - Antialiasing (enabled)", 20, 160);
	else
		render_text(p, "U - Antialiasing (disabled)", 20, 160);

}

void		render_text(t_params *p, char *str, int x, int y)
{
	SDL_Color	color;
	SDL_Surface	*text;
    SDL_Rect Message_rect; //create a rect

    Message_rect.x = x;  //controls the rect's x coordinate 
    Message_rect.y = y; // controls the rect's y coordinte
    Message_rect.w = 100; // controls the width of the rect
    Message_rect.h = 100; // controls the height of the rect
	if (TTF_Init() != 0)
	{
		ft_putstr("FAIL TO LOAD TTF\n");
		exit(0);
	}
	if (p->font == NULL)
	{
		ft_putstr("FAIL TO LOAD FONT!\n");
		exit(0);
	}
	color = (SDL_Color){255, 255, 0, 0};
	text = TTF_RenderText_Solid(p->font, str, color);
    p->surface->locked = 0;
	SDL_BlitSurface(text, NULL, p->surface, &Message_rect);
	SDL_FreeSurface(text);
	SDL_UpdateWindowSurface(p->win_ptr);
}
