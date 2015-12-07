# ifndef EIGEN_H_
# define EIGEN_H_

#include "../DataBase/base.h"

void mean_images(EltDB *elt, int *mean);

void difference(int *set, int *mean, int nb);

int *transposee(int *set, int nb);

int *covariance(int *set, int nb);

# endif
