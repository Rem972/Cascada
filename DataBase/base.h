#ifndef BASE_H_
#define BASE_H_

typedef struct EltDB
{
    int eigen;
    char *name;
    struct EltDB *next;
    struct EltDB *old;
    
} EltDB;

void initializeEltDB(EltDB *begin);

char* catstr(char* new, char str, char* s);

void Update(EltDB *begin);

int ajout_eltDB(EltDB *begin, gchar *filename);


#endif
