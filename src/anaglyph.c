
#include "../inc/rtv1.h"

void		render1(t_params	*p)	
{
	int		x;
	int		y;
	float	u;
	float	v;
	int		tmp;
	int		i;
	int		j; 

	y = 0;
	while (y < p->height)
	{
		x = 0;
		while (x < p->width)
		{
			u = ((float)x + 0.5) / (float)p->width;
			v = ((float)y + 0.5) / (float)p->height;
			tmp = get_color(ray_trace(u, v, p));
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
}

Uint32	map_rgb(t_vec rgb)
{
	if ((int)rgb[0] > 255)
		rgb[0] = 255;
	if ((int)rgb[1] > 255)
		rgb[1] = 255;
	if ((int)rgb[2] > 255)
		rgb[2] = 255;
	return (((int)rgb[0] << 16) + ((int)rgb[1] << 8) + (int)rgb[2]);
}

SDL_Surface *blend_surfaces(SDL_Surface *sur, SDL_Surface *sur1)
{
    SDL_Surface *res;
    Uint32      *p_sur;
    Uint32      *p_sur1;
    Uint32      *p_res;
    t_vec       c_sur;
    t_vec       c_sur1;
    t_vec       c_res;
    int y;
    int x;
    res = SDL_CreateRGBSurface(0, sur->w, sur1->h, 32, 0, 0, 0, 0);
    p_sur = sur->pixels;
    p_sur1 = sur1->pixels;
    p_res = res->pixels;
    SDL_LockSurface(sur);
    SDL_LockSurface(sur1);
    SDL_LockSurface(res);
    y = 0;
    while (y < sur->h)
    {
        x = 0;
        while (x < sur->w)
        {
            c_sur = int_to_color(p_sur[x + y * sur->w]);
            c_sur1 = int_to_color(p_sur1[x + y * sur1->w]);
            c_res[0] = (c_sur[0] + c_sur1[0]) < 255 ? c_sur[0] + c_sur1[0] : 255;
            c_res[1] = (c_sur[1] + c_sur1[1]) < 255 ? c_sur[1] + c_sur1[1] : 255;
            c_res[2] = (c_sur[2] + c_sur1[2]) < 255 ? c_sur[2] + c_sur1[2] : 255;
            p_res[x + y * sur->w] = map_rgb(c_res);
            x++;
        }
        y++;
    }
    SDL_UnlockSurface(sur);
    SDL_UnlockSurface(sur1);
    SDL_UnlockSurface(res);
    return (res);
}

void red_blue_version(SDL_Surface *sur, int color, SDL_Surface *orig)
{
    Uint32 *pixels;
    Uint32 *pixels1;

    int x;
    int y;
    t_vec col;
    t_vec col1;
    SDL_LockSurface(sur);
    SDL_LockSurface(orig);
	pixels = (Uint32 *)sur->pixels;
	pixels1 = (Uint32 *)orig->pixels;
    y = 0;
    while (y < sur->h)
    {
        x = 0;
        while (x < sur->w)
        {
            col = int_to_color(pixels[x + y * sur->w]);
            col1 = int_to_color(pixels1[x + y * orig->w]);
            col[0] = (0.21 * col1[0] + 0.72 * col1[1] + 0.07 * col1[2]) / 2;
            col[1] = col[0];
            col[2] = col[0];
            if (color == RED){
                col[0] *= 4;
            }
            if (color == BLUE){
                col[2] *= 4;
            }
            pixels[x + y * sur->w] = map_rgb(col);
            x++;
        }
        y++;
    }
    SDL_UnlockSurface(sur);
    SDL_UnlockSurface(orig);
}

void anaglyph_render(t_params *p)
{
    SDL_Surface *Red;
    SDL_Surface *Blue;

    Red = SDL_CreateRGBSurface(0, p->width, p->height, 32, 0, 0, 0, 0);
    Blue = SDL_CreateRGBSurface(0, p->width, p->height, 32, 0, 0, 0, 0);
    camera_move(3, p);
	render1(p);
    SDL_BlitSurface(p->surface, NULL, Red, NULL);
    red_blue_version(Red, RED, p->surface);
    camera_move(2, p);
	render1(p);
    SDL_BlitSurface(p->surface, NULL, Blue, NULL);
    red_blue_version(Blue, BLUE, p->surface);
    SDL_Surface *Blended = blend_surfaces(Red, Blue);
    SDL_BlitSurface(Blended, NULL, p->surface, NULL);
    SDL_FreeSurface(Red);
    SDL_FreeSurface(Blue);
    SDL_FreeSurface(Blended);
    render_screen(p);
}

void black_n_white(t_params *p)
{
    SDL_Surface *new_sur;
    int         x;
    int         y;
    t_color     color;
    t_color     old_color;
    Uint32      *pixels;
    Uint32      *old_pixels;
    y = 0;
    new_sur = SDL_CreateRGBSurface(0, p->width, p->height, 32, 0, 0, 0, 0);
    SDL_BlitSurface(p->surface, NULL, new_sur, NULL);
    SDL_LockSurface(new_sur);
    SDL_LockSurface(p->surface);
    pixels = (Uint32 *)new_sur->pixels;
    old_pixels = (Uint32 *)p->surface->pixels;
    while(y < new_sur->h)
    {
        x = 0;
        while (x < new_sur->w)
        {
            color = int_to_color(pixels[y * new_sur->w + x]);
            old_color = int_to_color(old_pixels[y * new_sur->w + x]);
            color[0] = (0.21 * old_color[0] + 0.72 * old_color[1] + 0.07 * old_color[2]);
            color[1] = color[0];
            color[2] = color[0];
            pixels[y * new_sur->w + x] = map_rgb(color);
            x++;
        }
        y++;
    }
    SDL_UnlockSurface(new_sur);
    SDL_UnlockSurface(p->surface);
    SDL_BlitSurface(new_sur, NULL, p->surface, NULL);
    SDL_FreeSurface(new_sur);
    render_screen(p);
}
