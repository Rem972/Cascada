# ifndef ADABOOST_H_
# define ADABOOST_H_

#include <math.h>
void swap(int* a, int*b);
typedef struct
{
    char *filename; //Haar characs computed for the sample img
    int positive; // 0 is not a face , 1 is a face 
} Sample;
typedef struct
{
    int treshold;
    double alpha;
    double error;
    int index; //which feature is selected 
    int polarity; 
} WeakClassifier;
typedef struct 
{
  
    int count;
    WeakClassifier wc[25];

} StrongClassifier;
    
double tabSum(double *tab, int len);
int getFeature(Sample *samples,int *values, int featNumber, int nbSamples);
int testValue(int value, int threshold, int p);
double computeSum(int *values, double *weights, Sample samples[],
                  int threshold,int N,int p);
int findTreshold(int *values, double *weights, Sample samples[],
                 int N, double range);
void AddClassifier(StrongClassifier sc, WeakClassifier wc);
StrongClassifier adaBoost(Sample samples[], int nbPos, int nbNeg
            , int T, int NbFeatures);

int applyStrongClassifier(StrongClassifier strong, Sample sample, double blc);



#endif

