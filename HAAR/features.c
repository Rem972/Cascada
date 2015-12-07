#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "compute.h"

//DW = fenêtre de détection
//ii = image intégrale

void changeparameters(int *w, int *h, int *incw, int *inch, int type)
{
    if (type == 1)
        *w = 1, *h = 0, *incw = 2, *inch = 1;
    if (type == 2)
        *w = 2, *h = 0, *incw = 3, *inch = 1;
    if (type == 3)
        *w = 0, *h = 1, *incw = 1, *inch = 2;
    if (type == 4)
        *w = 0, *h = 2, *incw = 1, *inch = 3;
    if (type == 5)
        *w = 1, *h = 1, *incw = 2, *inch = 2;
}

void haarFeatures(int ii[], int width, int height, char *filename)
{
    FILE *file = fopen(filename, "w+");
    if (file == NULL)
        printf("ERROR OPENING FILE\n");

    int c = 24, ori, res, err, w2, h2, incw, inch;

    //La taille de la DW doit être <= à la taille de l'ii
    while (c <= width && c <= height)
    {
        //Toutes les positions possibles de la DW
        for (int j = 0; j + c <= height; j++)
        {
            for (int i = 0; i + c <= width; i++)
            {
                for (int type = 1; type < 6; type++)
                {
                    changeparameters(&w2, &h2, &incw, &inch, type);
                    //fprintf(file, "(%d, %d, %d, %d) :\n", type, i, j, c);
                    //Différentes tailles du feature
                    for (int h = h2; h < c; h += inch)
                    {
                        for (int w = w2; w < c; w += incw)
                        {
                            //Positions du feature
                            for (int y = 0; y + h < c; y++)
                            {
                                for (int x = 0; x + w < c; x++)
                                {
                                    ori = (j + y)*width + i + x;
                                    res = compute(ii, type, w, h, width, ori);

                                    err = fprintf(file, "%d\n", res);
                                    if (err == 0)
                                        printf("Error writing in HAARCARACS");
                                }
                            }
                        }
                    }
                }
            }
        }

        c = c*5/4;
    }
    fclose(file);
}


