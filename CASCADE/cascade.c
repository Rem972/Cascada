#include<stdlib.h>
# include<stdio.h>
# include<err.h>
# include "../Adaboost/adaboost.h"
# include "cascade.h"
# include <string.h>

void swap2 (Sample *a, Sample *b)
{
    Sample c = *a;
    *a = *b;
    *b = c;
}

void writeSam(Sample samples[], int nbPos, int nbNeg)
{
    for (int i=0; i < nbPos + nbNeg; i++)
        printf("| %s", samples[i].filename);
    printf("| \n\n");
}

void write(StrongClassifier strong)
{
    FILE *f = fopen("SClist", "a");
    for(int i = 0; i < strong.count; i++) 
    {
        fprintf(f,"%d\n%f\t"
        "\n%d\n%f\n%d\n\n ",
        strong.wc[i].index,strong.wc[i].error,
        strong.wc[i].treshold,
        strong.wc[i].alpha,
        strong.wc[i].polarity);
    }
    fprintf(f,"!\n"); 
    fclose(f);
}

int sampleUp(StrongClassifier strong, Sample samples[], int nbPos, int nbNeg, 
int k)
{
    //Sample sam[] = sizeof(Sample) * (nbPos + nbNeg + 1);
    /*for (int i=1; i<=nbPos; i++)
    {
        sam[i] = samples[i];
    }*/
    int i = nbPos;
    int j = nbPos;
    while (i < nbNeg + nbPos)
    {
        //printf("SAMPLEUP\n");
        if(!applyStrongClassifier(strong, samples[i], k))
            nbNeg--;
        else
            samples[j++] = samples[i];
        i++;
    }
    //free (samples[])
    //samples[]=sam[];
    return nbNeg;
}


float rateSetter(StrongClassifier strong, Sample samples[], int nb, int nb2, int
j)
{
    float a = 0;
	int k;
    for (int i = 0; i < nb; i++)
    {
        printf("RATE, %d, %d\n",i,nb2);  
        k = applyStrongClassifier(strong, samples[i + nb2], j);
        a= ( a * i + k) / ( i + 1);
        printf("k, %d a, %f ,i %d \n",k,a,i);
    }
    return a;
}


void cascade (Sample samp[], int nbPos, int nbNeg, float f, float FTarget, float D)
{
    /* f correspond au nombre maximum de faux positif
     * FTarget est le nombre
     * D est le taux minimum de detection
     */
    int etape = 3;
    float FMinus, FCurrent, dMinus, d;
    FMinus = 1;
    dMinus = 1;
    // Initialisation des F(n-1), d(n-1), F(n) et d(n) 
    int nFeat = 2;
    int neg = nbNeg;
    printf("1er tableau\n");
    writeSam(samp, nbPos, nbNeg);
        for (int i = 0; i < etape; i++)
        {
            nbNeg = neg;
            Sample *samples = malloc(sizeof (Sample) * (nbPos + nbNeg));
            for (int a = 0; a < nbPos + nbNeg; a++)
                samples[a] = samp[a];
            //printf("\n1er tableau %d\n", i);
            //writeSam(samples, nbPos, nbNeg);
            StrongClassifier strong;
            do 
            {
                FCurrent = FMinus;
                do
                {
                    nFeat++;
                    //StrongClassifier strong with n[j] feature;
                    printf("\nTableau %d, %d, %d\n", i, nbPos, nbNeg);
                    writeSam(samples, nbPos, nbNeg);
                    strong = adaBoost(samples, nbPos, nbNeg, 
                            nFeat, 162336);
                    //Operation taux de faux positif courrant
                    d = rateSetter(strong, samples, nbPos, 0, 2);
                    FCurrent = rateSetter(strong, samples, nbNeg, nbPos, 2);
                    int j = 2;
                    while(D * dMinus > d)
                    {
                        printf("dMinus, %f d, %f\n",dMinus,d);
                        d = rateSetter(strong, samples, nbPos, 0, j++);
                    }
                    printf("\nd, %f\n",d);
                    FCurrent = rateSetter(strong, samples, nbNeg, nbPos, j);
                    nbNeg = sampleUp(strong, samples, nbPos, nbNeg,  j);
                    FMinus = FCurrent;
                    dMinus = d;
                }while(FCurrent > f*FMinus);
            }while(FCurrent > FTarget);
        write(strong);
        printf("\nTableauSortie %d\n", i);
        writeSam(samples, nbPos, nbNeg);
        free(samples);
    }
     printf("END\n");
}

