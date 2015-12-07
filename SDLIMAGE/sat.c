# include <stdlib.h>
# include <stdio.h>
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include "pixel_operations.h"
# include "basic_fun.h"

void img_integrale(SDL_Surface *img,int *sat)
{
    convert_to_grey(img);

    for(int i = 0; i < img->h; i++)
    {   
        int offset = i*(img->w);
        for(int j = 0; j < img->w; j++)
        {
          Uint8 r, g, b;
          SDL_GetRGB(getpixel(img,i,j), img->format, &r, &g, &b);
          int pixel2 = (int)r;

          if(i == 0 && j > 0)
              pixel2 += sat[j - 1];

          if(j == 0 && i > 0)
              pixel2 += sat[offset - img->w];

          if(i > 0 && j > 0)
             
               pixel2 += sat[offset + j - img->w] + sat[offset + j - 1]
                        - sat[offset + j - img->w - 1];

         sat[offset + j] = pixel2; 
        }
    }
    
} 
