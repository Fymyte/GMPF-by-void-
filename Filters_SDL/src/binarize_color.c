#include "../headers/binarize_color.h"

void binarize_color(SDL_Surface* image_surface , SDL_Surface* screen_surface)
{
	int width = image_surface->w;
	int height = image_surface->h;

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			Uint32 pixel = get_pixel(image_surface, i, j);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			if (r > 127)
				r = 255;
			else
				r = 0;
			if (g > 127)
				g = 255;
			else
				g = 0;
			if (b > 127)
				b = 255;
			else
				b = 0;

			pixel = SDL_MapRGB(image_surface->format, r, g, b);
			put_pixel(image_surface, i, j, pixel);
		}
	}

	update_surface(screen_surface, image_surface);
}

/*int main()
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image("my_image.jpg");

    screen_surface = display_image(image_surface);

    wait_for_keypressed();
    binarize_color(image_surface, screen_surface);
    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

    return 0;
}*/

