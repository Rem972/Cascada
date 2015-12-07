# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <string.h>
# include <errno.h>
# include "adaboost.h"

//# define T 20 //nb de classifieurs faibles dans un fort
//# define NbFeatures 162336
//# define MAXFEATVAL 10000 

double tabSum(double *tab, int len)
{
    double acc = 0;
    for(int i = 0; i < len; i++)
        acc += tab[i];
    return acc;
        
}

double min(double a, double b)
{
    return a <= b ? a : b;
}

double max(double a, double b)
{
    return a >= b? a : b;
}


void swap(int *a, int *b)
{
    int c = *a;
    *a = *b;
    *b = c;
} 

int* choose_pivot(int *begin, int *end)
{
    int *p = (begin + (end-begin)/2);
    return p;
}

int* partition(int *begin, int *end, int *pivot)
{
    int pval = *pivot;
    swap(pivot, end - 1);
    pivot = begin;
    for(int *i = begin; i < end - 1; i++)
    {
        if(*i < pval)
        {
            swap(pivot,i);
            pivot++;
        }
    }
    swap(pivot, end - 1);
    return pivot;
} 

void quick_sort(int *begin, int *end)
{
    if(end - begin > 1)
    {
        int *pivot = choose_pivot(begin, end);
        pivot = partition(begin, end, pivot);
        quick_sort(begin, pivot);
        quick_sort(pivot + 1, end);
    }
}

int getFeature(Sample *samples, int *values, int featNumber, int nbSamples)
{
    for(int i = 0; i < nbSamples; i++)
    {
       FILE *file = fopen(samples[i].filename, "r"); 
    if(file == NULL)
    {   
       // printf("Error opening : %s\n",featFilePath);
        printf("%s\n",strerror(errno));
        return -1;
    }
    char *s = malloc(10*sizeof(char));
    s = fgets(s,10,file);
    for(int i = 0; i < featNumber; i++)
    {
        s = fgets(s,10,file);
    }
    int res = atoi(s);
    free(s);
    s = NULL;
    values[i] = res;
    fclose(file);
    
    }
    return 0;

    

}

int featVal(FILE *file)
{
    char *s = malloc(10 * sizeof(char));
    s = fgets(s, 10, file);
    int res = atoi(s);
    free(s);
    s = NULL;
    return res;
    
}

int testValue(int value, int threshold,int p)
{
    if(p*value < p* threshold)
        return 1;
    else
        return 0; 
}

//somme de 1 à N
double computeSum(int *values, double *weights, Sample samples[],
                  int threshold,int N,int p)
{
    double acc = 0.0;
    for(int i = 0; i < N; i++)
    {
        int tested = testValue(values[i],threshold,p);
        acc += weights[i]*fabs((double)tested - samples[i].positive);
    }
    return acc;
}

// Trouve le seuil qui minimise l'erreur de classification
int findTreshold(int *values, double *weights, Sample samples[],
                 int N, double range)
{
 
    
    int min1 = 2000000;
    int min2= 0;
   
    int first = -15000;
    int last  =  1000;
    for(int i = first; i < last ; i++)
    {
        double resPos = computeSum(values,weights,samples,i,N,1);
        double resNeg = 3;
         resNeg = computeSum(values, weights, samples,i,N,-1);
        double res = min(resPos, resNeg);
       if(res == range)
          min2 = i; 
        if(res < min1)
        {
            min1 = res;
            min2 = i;
        }
    }

    return min2;

}

void AddClassifier(StrongClassifier sc, WeakClassifier wc)
{
    sc.wc[sc.count] = wc;
    sc.count++;
}

StrongClassifier adaBoost(Sample samples[], int nbPos, int nbNeg
            ,  int T, 
            int NbFeatures)
{
    /* Le nombre de classifieurs faibles dans un fort correspond
     * à T. NbFeatures est le nombre de haar-features calculées
     * pour une fenêtre de détection x*x
     */
    int nbSamples = nbPos + nbNeg;
    double *weights = malloc(nbSamples * sizeof(double));
    int *usedFeature = malloc(NbFeatures * sizeof(int));

        
    StrongClassifier strong;
    strong.count = 0;
   
    int i,j;

    for(i = 0; i < NbFeatures; i++)
        usedFeature[i] = 0;

    //initialisation des poids
    for(i = 0; i < nbSamples; i++)
        if(samples[i].positive == 1 && nbPos)
            weights[i] = 1.0/nbPos;
        else
        {  
            if(nbNeg)
                weights[i] = 1.0/nbNeg;
        }

    //Boucle principale (boosting)
    for(int t = 0; t < T; t++)
    {
        FILE *files[nbSamples];
        for(i = 0; i < nbSamples; i++)
            files[i] = fopen(samples[i].filename, "r");

        //Normalisation des poids
        double summedWeightsPos = tabSum(weights, nbPos);
        double summedWeightsNeg = tabSum(weights + nbPos, nbSamples);
        for(i = 0; i < nbSamples; i++)
        {
            if(samples[i].positive)
                weights[i] /= summedWeightsPos;
            else
                weights[i] /= summedWeightsNeg;
        }
        
        //Fin de la normalisation des poids

        //Pour chaque feature on trouve train
        //un classifieur faible hj
        //On trouve ensuite le classifieur
        //Avec le taux d'erreur minimal
        WeakClassifier *classifiers = malloc(sizeof(WeakClassifier) * NbFeatures);

        for(j = 0; j < NbFeatures; j++)
        {
            if(usedFeature[j])
            {
                classifiers[j].error = 42;
                continue;
            }
               
             
            
            int *featValues = malloc(sizeof(int) * nbSamples);
            for(i = 0; i < nbSamples; i++)
                featValues[i] = 0;
            
            for(i = 0; i < nbSamples; i++)
                featValues[i] = featVal(files[i]);
                                         

            
            int sortedValues[nbSamples];
            int *sort = memcpy(sortedValues, featValues,
                                  nbSamples*sizeof(int));
            quick_sort(sort, sort + nbSamples);

            /*Do not try to understand this */

            double Tpos = 0.0;
            double Tneg = 0.0;
            double Spos = 0.0;
            double Sneg = 0.0;
            for(i = 0; i < nbPos; i++)
            {
                Tpos += weights[i];
                if(weights[i] < sortedValues[i])
                    Spos += weights[i];
            }
            for(; i < nbSamples; i++)
            {
                Tneg += weights[i];
                if(weights[i] < sortedValues[i])
                    Sneg += weights[i];
            }

            double range = min(Spos + (Tneg - Sneg), Sneg + (Tpos - Spos));                     
            
            int treshold = findTreshold(featValues, weights, samples,
                                        nbSamples,range);
            
            double errorPos = computeSum(featValues, weights, samples,
                                      treshold, nbSamples, 1);
            double errorNeg = 3;
            if(errorPos > 0.5)
                errorNeg = computeSum(featValues, weights, samples,
                                      treshold, nbSamples, -1);
            int polarity;
            double error;
            if(errorPos < errorNeg)
            {
                polarity = 1;
                error = errorPos;
            }
            else
            {
                polarity = -1;
                error = errorNeg;
            }
                

            classifiers[j].treshold = treshold;
            classifiers[j].error = error;
            classifiers[j].polarity = polarity;

            free(featValues);    
           // printf("%d\n",j); 

        }

        //On cherche le classifieur avec le taux
        //d'erreur minimal
        double minError = classifiers[0].error;
        int minIndex = 0;
        int treshold = classifiers[0].treshold;
        int polarity = 1;

        for(j = 1; j < NbFeatures; j++)
        {
            if(classifiers[j].error < minError && classifiers[j].error != 0)
            {
                treshold = classifiers[j].treshold;
                minError = classifiers[j].error;
                minIndex = j;
                polarity = classifiers[j].polarity;
                
            }
        }

       
        if(minError == 0)
        {
            printf("minError = 0\n");
            
        }

        //Coefficients ...
        double beta = minError/(1 - minError);
        double alpha = log(1/beta);

        usedFeature[minIndex] = 1;
        WeakClassifier cls;
        cls.treshold = treshold;
        cls.error = minError;   
        cls.alpha = alpha;
        cls.index = minIndex;
        cls.polarity = polarity;
       // AddClassifier(strong,cls);
        strong.wc[strong.count] = cls;
        strong.count++;
        

        //Pondération des poids 
        //des exemples d'apprentissage            
            int values[nbSamples];
            getFeature(samples,values, minIndex,nbSamples);
            for(i = 0; i < nbSamples; i++)
            {
                int e = testValue(values[i], treshold,polarity);

                weights[i] *= pow(beta, (double)(1-e));
            }

        

        printf("stage : %d j : %d\n",t,j);
        free(classifiers);
        for(i = 0; i < nbSamples; i++)
            if(files[i])
                fclose(files[i]);
    }
  /*  double sumAlpha = 0.0; 
    for(i = 0; i < T; i++)
        sumAlpha += strong.wc[i].alpha;
    sumAlpha /= 2.0;

    double sumCls = 0.0;
    for(i = 0; i < T; i++)
    {
        int val = testValue((int)strong.value, strong.wc[i].treshold);
        sumCls += strong.wc[i].alpha * val;

    } */

 /*   double sumCls = 0.0;
    double firstSumCls = 0.0;
    for(j = 0; j < nbSamples; j++)
    {
        sumCls = 0.0;
        for(i = 0; i < T; i++)
        {
        
          int val = getFeature(samples[j].filename,strong.wc[i].index);
          int clsValue = testValue(val,strong.wc[i].treshold);
          sumCls += strong.wc[i].alpha * clsValue;
        }
        
        if(j == 0)
            firstSumCls = sumCls;
        if(sumCls >= sumAlpha)
            printf("StrongClassifer applied to sample (%d) : 1\n",j);
        else
            printf("StrongClassifer applied to sample (%d) : 0\n",j);
    }   */

    free(weights);
    free(usedFeature);
    //fclose(test);

    return strong;

}

int applyStrongClassifier(StrongClassifier strong, Sample sample, double blc)
{
    int i;
    double sumAlpha = 0.0; 
    for(i = 0; i < strong.count; i++)
        sumAlpha += strong.wc[i].alpha;
    sumAlpha /= blc;
    printf("THRESHOLD = %f\n", sumAlpha);

        double sumCls = 0.0;
    
        for(i = 0; i < strong.count; i++)
        {
           int val = 0; 
           getFeature(&sample,&val,strong.wc[i].index,1);
          int clsValue = testValue(val,strong.wc[i].treshold,
                         strong.wc[i].polarity);
          sumCls += strong.wc[i].alpha * clsValue;
        }
        
        return (sumCls >= sumAlpha);
    

}


