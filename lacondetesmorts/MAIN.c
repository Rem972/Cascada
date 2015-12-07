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
    SDL_Surface *img = load_image("img/McCord2.jpg");
    SDL_Surface *img2= load_image("img/tamere2.jpg");
    SDL_Surface *img3= load_image("img/bleu.jpg");
    SDL_Surface *img4= load_image("img/tete.jpg");
    SDL_Surface *img5= load_image("img/hs10.png");

    int *matrix = malloc(img->h*img->w*sizeof(int));
    int *matrix2 = malloc(img2->h*img->w*sizeof(int));
    int *matrix3 = malloc(img3->h*img->w*sizeof(int));
    int *matrix4 = malloc(img4->h*img->w*sizeof(int));
    int *matrix5 = malloc(img5->h*img->w*sizeof(int));
    img_integrale(img, matrix);
    img_integrale(img2,matrix2);
    img_integrale(img3,matrix3);
    img_integrale(img4,matrix4);
    img_integrale(img5,matrix5);
    haarFeatures(matrix, img->w, img->h,"test1");
    haarFeatures(matrix2,img2->w,img2->h,"test2");
    haarFeatures(matrix3,img3->w,img3->h,"test3");
    haarFeatures(matrix4,24,24,"test4");
    haarFeatures(matrix5,24,24,"test5");
    //FILE *f = fopen("ADATEST", "a");

    Sample sample[5];
    //for(int i = 0; i < 5; i++)
      //  sample[i] = malloc(sizeof(sample));
    char file[] = "test1";
    char file2[]= "test2";
    char file3[]= "test3";
    char file4[]= "test4";
    char file5[]= "test5";
    sample[0].filename = file;
    sample[0].positive = 1;
    sample[1].filename = file2;
    sample[1].positive = 1;
    sample[2].filename = file4;
    sample[2].positive = 1;
    sample[3].filename = file3;
    sample[3].positive = 0;
    sample[4].filename = file5;
    sample[4].positive = 0;

  // StrongClassifier strong = adaBoost(sample,3,2,5,162336);
   cascade (sample, 3, 2, 0.5, 0.5, 0.5);
   /* for(int i = 0; i < strong.count; i++)
    {
        fprintf(f," WEAK CLASSIFIER %d :\nINDEX : %d\nERROR :%f\nTHRESHOLD : %d\n ALPHA : %f\n POLARITY:%d\n\n ",i,
                strong.wc[i].index,strong.wc[i].error,
                strong.wc[i].treshold,
                strong.wc[i].alpha,
                strong.wc[i].polarity);
    }

    int boole = applyStrongClassifier(strong, sample[0], 2);
    printf("%d\n",boole);

    fclose(f);*/
    free(matrix);
    free(matrix2);
    free(matrix3);

    return 0;
    
    
}

