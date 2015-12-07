#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <glib/gdir.h>
#include <string.h>
#include "base.h"

void initializeEltDB(EltDB *begin)
{
    EltDB *temp = begin;
    temp->eigen = 0;
    begin->next = malloc(sizeof(EltDB));
    begin->old = NULL;
    begin = begin->next;
    char *path = "../img/faces/yalefacesgood";  
    GDir* dir = g_dir_open(path, 0, NULL);
    G_CONST_RETURN gchar* file = g_dir_read_name(dir);
    FILE *fichier = fopen("test","r+");
    while (file != NULL && begin)
    {
        (temp->eigen)++;
        begin->name = (char*)file;
        fputs(begin->name,fichier);
        fputc('/',fichier);
        fputs("2555",fichier);
        fputc('\n',fichier);
        begin->next = malloc(sizeof(EltDB));
        begin = begin->next;
        begin->old = begin;
        file = g_dir_read_name(dir);
    }
    begin->next = NULL;
    begin =temp;  
}
char* catstr(char* new,char str,char* s)
{
    int i = 0;
    while(*new != '\0')
    {
        s[i] = *new;
        i++;
        new++;
    }
    s[i] = str;
    s[i+1] = '\0';
    return s;
}
void Update(EltDB *begin)
{
    EltDB *temp = begin;
    temp->eigen = 0;
    begin->next = malloc(sizeof(EltDB));
    begin = begin->next;
    char s[99];
    char str;
    char *new = malloc(sizeof(char)*50);
    new = "";
    FILE *fichier = fopen("test","r+");
    FILE *file = fopen("test_final","r+");
    while ((str = fgetc(fichier))!=EOF)
    {      
        if(str == '/')
        {
            begin->name = new;
            fputs(begin->name,file);
            fputc('/',file);
            new = "";
        }
        else if(str == '\n')
        {   
            (temp->eigen)++;
            begin->next = malloc(sizeof(EltDB));
            begin = begin->next;
            begin->old = begin;
            begin->eigen = atoi(new);
            fprintf(file,"%d\n",begin->eigen);
            new = "";
        }
        else
        {
           new = catstr(new,str,s);   
        }
    }
    begin->next = NULL;
    begin = temp;   
}

int ajout_eltDB(EltDB *begin, gchar *filename)
{
    FILE *file = fopen("test","a");
    EltDB *temp = begin->next;
    EltDB *new = malloc(sizeof(EltDB));
    while(temp->next != NULL && strcmp(temp->name,filename)<0)
    {
        temp = temp->next;  
    }
    if(strcmp(temp->name,filename) != 0)
    {
        strcpy(new->name, filename);
        new->next = temp;
        temp->old = new;
        (begin->eigen)++;
        fputs(new->name,file);
        return 1;
    }
    if(strcmp(temp->next->name,filename)!= 0)
    {
        strcpy(new->name,filename);
        new->next = temp->next;
        temp->next->old = new;
        (begin->eigen)++;
        fputs(new->name,file);
        return 1;
    }
    if(strcmp(temp->name,filename) == strcmp(temp->next->name,filename))
    {
        return 0;
    }
    strcpy(new->name, filename);
    fputs(new->name,file);
    new->next = NULL;
    new->old = temp->next;
    (begin->eigen)++;
    fputs(new->name,file);
    return 1;
}

/*int supp_eltDB(EltDB *begin, gchar *name)
{
    EltDB *temp = begin;
    while (temp != NULL && strcmp(temp->name,name)<0)
    {
        temp = temp->next;
    }
    if (temp == NULL || strcmp(temp->name,name)!=0)
    {
        return 0;
    }
    temp->old->next = temp->next;
    free(temp);
    return 1;  
}*/

/*EltDB* search_EltDB(EltDB *begin, gchar *name)
{
    EltDB *temp = begin;
    while(temp != NULL && strcmp(temp->name,name) < 0)
    {
        temp = temp->next;
    }
    return temp;
}*/

/*int main()
{
    EltDB *begin = malloc(sizeof(EltDB));
    initializeEltDB(begin);
    Update(begin);
}*/
