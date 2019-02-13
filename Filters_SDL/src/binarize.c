#include "../headers/binarize.h"

void binarize(SDL_Surface* image_surface , SDL_Surface* screen_surface)
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
			Uint8 average = (r + g + b) / 3;

			r = average;
			g = average;
			b = average;

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
    binarize(image_surface, screen_surface);
    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

    return 0;
}*/
