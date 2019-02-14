#include "../headers/convolution.h"

int check(int width, int height, int i, int j)
{
    if (i < 0 || j < 0 || i > width || j > height)
        return 0;
    return 1;
}

void Convolute_f(SDL_Surface* image_surface , SDL_Surface* screen_surface, double *mat)
{
    int width = image_surface->w;
	int height = image_surface->h;
    int x = 3;

    struct Img_rgb *img = init_img_rgb(width, height);

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			//Uint8 r, g, b;
			double r, g, b;
            r = g = b = 0;

			for (int k = -x / 2; k <= x/2; k++)
            {
                for(int l = -x / 2; l <= x/2; l++)
                {
                    if (check(width, height, i + k, j +l) == 1)
                    {
                        Uint32 pixel = get_pixel(image_surface,i + k, j + l);
                        Uint8 red, green, blue;
                        SDL_GetRGB(pixel,image_surface->format,&red,&green,&blue);
                        r += mat[l + x/2 + k + x/2] * red;
                        g += mat[l + x/2 + k + x/2] * green;
                        b += mat[l + x/2 + k + x/2] * blue;
                    }
                }
            }
            if (r > 255)
                r = 255;
            else if (r < 0)
                r = 0;

            if (g > 255)
                g = 255;
            else if (g < 0)
                g = 0;

            if (b > 255)
                b = 255;
            else if (b < 0)
                b = 0;
            
			//Uint32 p = SDL_MapRGB(image_surface->format, r, g, b);
			//put_pixel(image_surface, i, j, p);
		    Matrix_val(img -> red, i, j, r);
            Matrix_val(img -> green, i , j, g);
            Matrix_val(img -> blue, i , j, b);
        }
	}
    Img_rgb_to_Image(image_surface, img);
    free_img_rgb(img);
    update_surface(screen_surface, image_surface);

}

void Convolution(SDL_Surface* image_surface , SDL_Surface* screen_surface)
{
    int i;
    printf("Waiting for a number....\n");
    printf("0 : Edge Enhance\n");
    printf("1 : Sharpen\n");
    printf("2 : Blur\n");
    if (scanf("%i", &i) == EOF)
        errx(1, "Error, scanf()");

    double *mat = malloc(sizeof(double) * 9);
    switch (i)
    {
        case 0 : 
            printf("Edge Enhance\n");
            mat[0] = mat[1] = mat[2] = mat[3] = mat[5] = mat[6] = mat[7] = mat[8] = -1;
            mat[4] = 8;
            break;
        case 1 :  
            printf("Sharpen\n");
            mat[0] = mat[2] = mat[6] = mat[8] = 0;
            mat[1] = mat[3] = mat[5] = mat[7] = -1;
            mat[4] = 5;
            break;
        default : 
            printf("Blur\n"); 
            mat[0] = mat[1] = mat[2] = mat[3] = mat[4] = mat[5] = mat[6] = mat[7] = mat[8] = (double) 1/9;
            break;
    }
    Convolute_f(image_surface, screen_surface, mat);
    free(mat);
}



/*int main()
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image("my_image.jpg");

    screen_surface = display_image(image_surface);

    wait_for_keypressed();
    
    Convolute(image_surface, screen_surface);
    
    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

    return 0;
}*/
