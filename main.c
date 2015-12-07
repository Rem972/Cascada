#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "SDLIMAGE/pixel_operations.h"
#include "SDLIMAGE/basic_fun.h"
#include "SDLIMAGE/sat.h"
#include "HAAR/features.h"
#include "Adaboost/adaboost.h"
#include <string.h>
#include "CASCADE/cascade.h"

int main()
{
    init_sdl();
    Sample *samples = malloc(125 * sizeof(Sample));
    
    int  i, acc = 0;
    for(i = 0; i < 125; i++)
    {
        samples[i].filename = malloc(70 * sizeof(char));
    }
    for(i = 0; i < 75; i++)
    {
        samples[i].positive = 1;
    }
    for(i = 75; i < 125; i++)
        samples[i].positive = 0;
    for( i = 0; i < 9; i++,acc++)
    {
       
        sprintf(samples[i].filename, "img/faces/subject0%d.gif",i+1);
        

    }

    for(i = 9; i < 15; i++,acc++) //15
    {
        sprintf(samples[i].filename, "img/faces/subject%d.gif", i+1);
    }

    for(i = 0; i < 9; i++,acc++)
    {
      
        sprintf(samples[i+15].filename,"img/faces/subject0%d.happy.gif",i+1);
     
    
    }

    for(i = 9; i < 15; i++,acc++) //30
    {
      
        sprintf(samples[i +15].filename, "img/faces/subject%d.happy.gif",i+1);
     
    
    }

    for(i = 0; i < 9; i++,acc++)
    {
    
        sprintf(samples[i+30].filename, "img/faces/subject0%d.sad.gif",i+1);
   
  
    }

    for(i = 9; i < 15; i++,acc++) //45
    {
    
        sprintf(samples[i +30].filename, "img/faces/subject%d.sad.gif",i+1);
   
  
    }

    for(i = 0; i < 9; i++, acc++)
    {
    
        sprintf(samples[i+45].filename, "img/faces/subject0%d.glasses.gif",i+1);
   
  
    }
    
    for(i = 9; i < 15; i++,acc++) //60
    {
    
        sprintf(samples[i+45].filename, "img/faces/subject%d.glasses.gif",i+1);
   
  
    }

    for(i = 0; i < 9; i++,acc++)
    {
    
        sprintf(samples[i+60].filename, "img/faces/subject0%d.sleepy.gif",i+1);
   
  
    }

    for(i = 9; i < 15; i++,acc++) //75
    {
    
        sprintf(samples[i+60].filename, "img/faces/subject%d.sleepy.gif",i+1);
   
  
    }

    for(i = 0; i < 50; i++)
    {
        if(i < 10)
            sprintf(samples[i+75].filename, "img/nonfaces/frame-000%d.jpg",i);
        else
            sprintf(samples[i+75].filename, "img/nonfaces/frame-00%d.jpg",i);
    }

    for(i = 0; i < 125; i++)
    {
        SDL_Surface *img = load_image(samples[i].filename);
        if(!img)
            printf("Cant load img %d\n",i);
        int *matrix = malloc(24*24*sizeof(int));
        char file[50];
        sprintf(file, "file%d",i);
        img_integrale(img, matrix);
        haarFeatures(matrix, 24,24, file);
        sprintf(samples[i].filename, "file%d",i);
    }
    
    cascade(samples, 75, 25, 0.99, 0.65. 0.9);        

    free(samples);
    return 0;

       
}
