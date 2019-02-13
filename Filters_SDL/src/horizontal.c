#include "../headers/horizontal.h"

/*void Img_rgb_to_Image(SDL_Surface* image_surface, struct Img_rgb *img)
{
    int width = image_surface -> w;
    int height = image_surface -> h;
    double r, g, b;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            r = Matrix_IJ(img -> red, i, j);
            g = Matrix_IJ(img -> green, i , j);
            b = Matrix_IJ(img -> blue, i, j);
            Uint32 p = SDL_MapRGB(image_surface->format, (Uint8)r, (Uint8)g, (Uint8)b);
			put_pixel(image_surface, i, j, p);
        }
    }
}*/

void Horizontal(SDL_Surface* image_surface , SDL_Surface* screen_surface)
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
		    Matrix_val(img -> red, i, height - j - 1, r);
            Matrix_val(img -> green, i, height - j - 1, g);
            Matrix_val(img -> blue, i, height - j - 1, b);
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
    Horizontal(image_surface, screen_surface);
    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

    return 0;
}*/
