# include <stdlib.h>
# include <stdio.h>
# include <gtk/gtk.h>
# include <string.h>
# include "../DataBase/base.h"

gchar* get_filename(gchar* name)
{
    gchar *s = malloc(100*sizeof(gchar));
    int i = 0;
    gchar *new = name;

    for (; *new != '\0'; new++)
    {
        if (*new == '/')
            i = 0;
        else
        {
            s[i] = *new;
            i++;
        }
    }
    s[i] = '\0';

    return s;
}  

int goodformat(gchar *path)
{
    int len = strlen((char *)path);
    char *ext = malloc(3*sizeof(char));
    if (len > 3)
    {
      for (int i = 2; i >= 0; i--, len--)
        ext[i] = path[len - 1];
    
      return (strcmp(ext, "png") || strcmp(ext, "jpg") || strcmp(ext, "gif") || strcmp(ext, "bmp"));
    }

    return 0;
}

void ajouter_img(GtkWidget *bouton3, gpointer window)
{
    if (bouton3)
    {
        FILE* file = fopen("test1", "a");
        GtkWidget* dialog = gtk_file_chooser_dialog_new("Choose a file",
                                                        GTK_WINDOW(window),
                                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                                        GTK_STOCK_OK,GTK_RESPONSE_OK,
                                                        GTK_STOCK_CANCEL,
                                                        GTK_RESPONSE_CANCEL,
                                                        NULL);
        //gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),g_get_temp_dir());
        gtk_widget_show_all(dialog);
        gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
        if (resp == GTK_RESPONSE_OK)
        {
            gchar *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            if (goodformat(get_filename(path)))
            {
                char str[100];
                char *src = (char *)path;
                char *dst = "img/faces/yalefacesgood";
                sprintf(str, "cp -r %s %s", src, dst);
                if (system(str))
                    printf("Error in cp");
                EltDB *begin = malloc(sizeof(EltDB));
                Update(begin);
                ajout_eltDB(begin,get_filename(path));
                fputs(get_filename(path), file);
                fputc('\n', file);
            }
            else
              printf("Bad format of: %s", get_filename(path));
        }
        gtk_widget_destroy(dialog);
    }
}

void button_ajout(GtkWidget* window)
{
    // INIT
    GtkWidget *bouton = gtk_button_new_with_label("Ajouter à la DATABASE");
    GtkWidget *bouton2 = gtk_button_new_with_label("       Detecter Visage       ");
    GtkWidget *bouton3 = gtk_button_new_with_label("      JOINDRE FICHIER      ");
    
    //TABLE
    GtkWidget *table = gtk_table_new(3, 3, TRUE);
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(table));
    gtk_table_attach(GTK_TABLE(table), bouton, 3, 4, 3, 4, GTK_EXPAND, GTK_EXPAND, 0, 0);
    gtk_table_attach(GTK_TABLE(table), bouton2, 3, 4, 4, 5, GTK_EXPAND, GTK_EXPAND, 0, 0);
    gtk_table_attach(GTK_TABLE(table), bouton3, 3, 4, 5, 6, GTK_EXPAND, GTK_EXPAND, 0, 0);
    
    //EVENT
    g_signal_connect(bouton3, "clicked", G_CALLBACK(ajouter_img), window);
    gtk_widget_show_all(window);
    gtk_main();
}


void initialize_window(GtkWindow* fenetre)
{
    gtk_window_set_title(fenetre, "WhoIsWho");
    gint width = 800;
    gint height = 550;
    gtk_window_set_default_size(fenetre, width, height); // Taille par default(resize auto)
    gtk_window_resize(fenetre, width, height); // Taille actuelle
    gtk_window_set_position(fenetre, GTK_WIN_POS_CENTER); // Ouverture au milieu de l'écran
    gboolean icone = gtk_window_set_icon_from_file(fenetre, "Graphic_interface/logo.jpg", NULL);// tente de mettre une icone
    if(icone)
    {
        gtk_window_iconify(fenetre);//iconifier
    }
}

/*int main(int argc, char **argv)
{
    //VARIABLE
    gtk_init(&argc, &argv);
    GtkWidget* MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWindow* fenetre = GTK_WINDOW(MainWindow);
    
    //SETUP WINDOW
    
    initialize_window(fenetre);
          
    //EVENT
    g_signal_connect(G_OBJECT(MainWindow), "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    
    //gtk_container_add(GTK_CONTAINER(MainWindow),file_button);
    //gtk_widget_show_all(MainWindow); //show window
    //gtk_main();
    button_ajout(MainWindow);
    return EXIT_SUCCESS;
}*/
