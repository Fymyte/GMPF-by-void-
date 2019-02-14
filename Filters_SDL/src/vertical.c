#include "../headers/vertical.h"

void Vertical(SDL_Surface* image_surface , SDL_Surface* screen_surface)
{
    int width = image_surface->w;
	int height = image_surface->h;

    struct Img_rgb *img = init_img_rgb(width, height);
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
            Uint32 pixel = get_pixel(image_surface, i, j);
			Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		    Matrix_val(img -> red, width - i - 1, j, r);
            Matrix_val(img -> green, width - i - 1, j, g);
            Matrix_val(img -> blue, width - i - 1, j, b);
        }
	}
    Img_rgb_to_Image(image_surface, img);
    free_img_rgb(img);
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
    Vertical(image_surface, screen_surface);
    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

    return 0;
}*/
