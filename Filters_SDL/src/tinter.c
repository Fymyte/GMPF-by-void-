#include "../headers/tinter.h"

void tinter(SDL_Surface* image_surface , SDL_Surface* screen_surface)
{
    Uint8 r1, g1, b1;
    int factor;

    printf("Waiting for colors values ...\n");
    printf("Enter red value\n");
    if (scanf("%hhu", &r1) == EOF)
        errx(1, "Error, scanf()");
    
    printf("Enter green value\n");
    if (scanf("%hhu", &g1) == EOF)
        errx(1, "Error, scanf()");

    printf("Enter blue value\n");
    if (scanf("%hhu", &b1) == EOF)
        errx(1, "Error, scanf()");

    printf("Enter factor (between 0 and 100)\n");
    if (scanf("%i", &factor) == EOF)
        errx(1, "Error, scanf()");

    int width = image_surface->w;
	int height = image_surface->h;

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			Uint32 pixel = get_pixel(image_surface, i, j);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			r = r * (100 - factor) / 100 + r1 * factor / 100;
            g = g * (100 - factor) / 100 + g1 * factor / 100;
            b = b * (100 - factor) / 100 + b1 * factor / 100;
			
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
    tinter(image_surface, screen_surface);
    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

    return 0;
}*/
