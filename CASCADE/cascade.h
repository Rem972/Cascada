# ifndef CASCADE_H_
# define CASCADE_H_
 
# include <stddef.h>
# include "../Adaboost/adaboost.h"




void cascade (Sample samples[], int nbPos, int nbNeg, float f, float FTarget, float D);


float rateSetter(StrongClassifier strong, Sample samples[], int nb, int nb2, int
k);

int sampleUp(StrongClassifier strong, Sample samples[], int nbPos, int nbNeg,
int j);


# endif 
