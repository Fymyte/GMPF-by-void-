#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "headers/binarize_color.h"
#include "headers/binarize.h"
#include "headers/grayscale.h"
#include "headers/horizontal.h"
#include "headers/negative.h"
#include "headers/tinter.h"
#include "headers/vertical.h"
#include "headers/sdl_op.h"
#include "headers/convolution.h"
#include "headers/Img_rgb_op.h"

void ChooseEffect(SDL_Surface *image_surface, SDL_Surface *screen_surface)
{
    printf("Enter a number.\n");
    printf("1 : Grayscale\n");
    printf("2 : Binarize\n");
    printf("3 : Binarize color\n");
    printf("4 : Negative\n");
    printf("5 : Tinter\n");
    printf("6 : Convolution\n");
    printf("7 : Mirror - Horizontal\n");
    printf("8 : Mirror - Vertical\n");
    int i;
    if (scanf("%i", &i) == EOF)
        errx(EXIT_FAILURE, "Error, scanf()");

    switch(i)
    {
        case 1 : 
            grayscale(image_surface, screen_surface);
            break;
        case 2 : 
            binarize(image_surface, screen_surface);
            break;
        case 3 : 
            binarize_color(image_surface, screen_surface);
            break;
        case 4 : 
            negative(image_surface, screen_surface);
            break;
        case 5 : 
            tinter(image_surface, screen_surface);
            break;
        case 6 : 
            Convolution(image_surface, screen_surface);
            break;
        case 7 : 
            Horizontal(image_surface, screen_surface);
            break;
        case 8 : 
            Vertical(image_surface, screen_surface);
            break;
        default : 
            ChooseEffect(image_surface, screen_surface);
            break;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Need the path of the image.");
    SDL_Surface *image_surface;
    SDL_Surface *screen_surface;

    init_sdl();

    image_surface = load_image(argv[1]);
    screen_surface = display_image(image_surface);
    
    wait_for_keypressed();
    for(;;)
    {
        ChooseEffect(image_surface, screen_surface);
        printf("Press ENTER key to continue\n");
        getchar();
        getchar();
    }
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
    return EXIT_SUCCESS;
}
