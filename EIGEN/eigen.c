#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../DataBase/base.h"
#include "../SDLIMAGE/basic_fun.h"
#include "eigen.h"

int *mean_image(EltDB *elt, int *mean)
{
  EltDB *tmp = elt->next;
  int *set = malloc(elt->eigen*576*sizeof(int));
  char *path;
  int *array;
  int cp = 0;

  while (tmp)
  {
    path = "../img/";
    array = image_to_intarray(strcat(path, tmp->name));
    
    for (int j = 0; j < 24; j++)
      for (int i = 0; i < 24; i++)
        *(set + cp*576 + j*24 + i) = *(array + j*24 + i);

    tmp = tmp->next;
    cp++;
  }

  for (int j = 0; j < 24; j++)
  {
    for (int i = 0; i < 24; i++)
    {
      *(mean + j*24 + i) = 0;
      for (int k = 0; k < cp; k++)
        *(mean + j*24 + i) += *(set + k*576 + j*24 + i);

      *(mean + j*24 + i) /= cp;
    }
  }

  return set;
}

void difference(int *set, int *mean, int nb)
{
  for (int j = 0; j < 24; j++)
    for (int i = 0; i < 24; i++)
      for (int k = 0; k < nb; k++)
        *(set + k*576 + j*24 + i) = *(set + k*576 + j*24 + i)
                                  - *(mean + j*24 + i);
}

int *transposee(int *set, int nb)
{
  int *trans = malloc(nb*576*sizeof(int));
  for (int j = 0; j < 24; j++)
    for (int i = 0; i < 24; i++)
      for (int k = 0; k < nb; k++)
        *(trans + k*576 + j*24 + i) = *(trans + k*56 + i*24 + j);

  return trans;
}

int *covariance(int *set, int nb)
{
  int *covar = malloc(576*sizeof(int));
  int *trans = transposee(set, nb);

  for (int j = 0; j < 24; j++)
    for (int i = 0; i < 24; i++)
    {
      for (int k = 0; k < nb; k++)
        for (int l = 0; l < 24; l++)
          *(covar + j*24 + i) += *(set + k*576 + j*24 + i + l)
                               * *(trans + k*576 + j*24 + i + l);
      *(covar + j*24 + i) /= nb;
    }

  return covar;
}

/*int *covariance(int *set, int nb)
{
  int *trans = transposee(set, nb);
  int *covar = malloc(576*sizeof(int));

  for (int j = 0; j < 24; j++)
    for (int i = 0; i < 24; i++)
    {
      *(covar + j*24 + i) = 0;
      for (int k = 0; k < nb; k++)
        *(covar + j*24 + i) += *(set + k*576 + j*24 + i

  return covar;
}*/

int isdetz(int *covar, int lambda)
{
  int det[24][24];

  for (int j = 0; j < 24; j++)
    for (int i = 0; i < 24; i++)
      det[i][j] = *(covar + j*24 + i);

  det = 1;
  for (int j = 0; j < 23; j++)
  {
    int i = j + 1;
    if (det[j][j] == 0)
    {
      int ok = 1;
      for (; i < 24 & !det[i][j]; i++);
      if (i == 24)
        det = 0;
    
      if (i < 24)
      {
        for (int g = 0; g < 24; g++)
          det[i][g] *= -1;
      
        int temp;

        for (int k = 0; k < 24; k++)
        {
          temp = det[i][k];
          det[i][k] = det[j][k];
          det[j][k] = temp;
        }
      }
    }
    else
    {
      for (; i < 24; i++)
      {
        for (int g = 0; g < 24; g++)
        {




int main()
{
  EltDB *sent = malloc(sizeof(EltDB));
  int nb = sent->eigen;
  int *mean = malloc(576*sizeof(int));
  int *set = mean_image(sent, mean);

  difference(set, mean, nb);
  int *trans = transposee(set, nb);
  return 0;
}
