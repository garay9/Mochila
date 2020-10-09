#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int rows = 1;
int cols = 3;
GtkWidget *window;
GtkBuilder *builder;
GtkWidget *objectsScroll;
GtkWidget *dynamicScroll;
GtkWidget *objectsGrid;
GtkWidget *dynamicGrid;

GtkWidget *addButton;
GtkWidget *removeButton;
GtkWidget *saveFileButton;
GtkWidget *chooseFileButton;

GtkWidget *cleanButton;
GtkWidget *executeButton;

extern void greedy1(struct Objeto *objetos, int size, int pesoMax);
extern void greedy2(struct Objeto *objetos, int size, int pesoMax);
extern void dinamic(struct Objeto *objetos, int productos, int pesoMax);

struct Objeto
{
    int valor;
    int costo;
    int cantidad;
    float relacion;
};


/*********************************
 *                               *
 *                               *
 *              DATA             *
 *                               *
 *********************************/

double** getData(){
    double **resultData = (double**) malloc(sizeof(double*)* (rows-1) * cols);
    GtkSpinButton* spin;
    for(int i = 1; i < rows; i++){
        printf("valor %d\n", sizeof(double)*cols);
        resultData[i] = (double*) malloc(sizeof(resultData[i]) * cols);
        for(int j = 0; j < cols; j++){
            spin = GTK_SPIN_BUTTON(gtk_grid_get_child_at (objectsGrid, j, i));
            resultData[i-1][j] = gtk_spin_button_get_value (spin);
            printf("Valor %f  ", resultData[i-1][j]);
        }
        printf("\n");
    }
    return resultData;
}

void runAlgorithms(double **matrix){
    GdkColor colorBg;
    GdkColor colorFg;
    gdk_color_parse ("#BA11FF", &colorBg);
    gdk_color_parse ("white", &colorFg);
    int size = (rows) * 50;
    struct Objeto *elements = malloc(size);
    struct Objeto objeto;
    GtkWidget *label;
    for(int  i = 0; i < rows -1 ; i++){
 
 
        objeto.costo = (int) matrix[i][0]; 

        objeto.valor = (int) matrix[i][1];

        objeto.cantidad = (int) matrix[i][2];

        objeto.relacion = ((float) objeto.valor / (float) objeto.costo);

       elements[i] = objeto;
        char str[48];
        
        char temp[48];
        strcpy(str, "x");
        sprintf(temp, "%d", i);
        strcat(str, temp);
        label =  gtk_label_new (str); 
        gtk_widget_modify_bg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &colorBg);
        gtk_widget_modify_fg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &colorFg);
        gtk_widget_set_size_request(label, 50, 40);
        gtk_grid_attach (GTK_GRID(dynamicGrid), label, i, 0,1,1);
        
    }

    gtk_widget_show_all(dynamicGrid);   
    dinamic(elements, (rows - 1), 8);
    free(elements);
}


void on_window_main_destroy()
{
    gtk_main_quit();
}

void addButton_clicked_cb(GtkButton *b){
   // GtkAdjustment *adjustment = ;
    gtk_grid_attach (GTK_GRID(objectsGrid), gtk_spin_button_new(gtk_adjustment_new(0, -1, 99, 1, 1, 1), 1, 3), 0, rows,1,1);   
    gtk_grid_attach (GTK_GRID(objectsGrid), gtk_spin_button_new(gtk_adjustment_new(0, -1, 99, 1, 1, 1), 1, 3), 1, rows,1,1);
    gtk_grid_attach (GTK_GRID(objectsGrid), gtk_spin_button_new(gtk_adjustment_new(0, -1, 99, 1, 1, 1), 1, 3), 2, rows,1,1);
    rows++;
    printf("Rows: %d\n", rows);
    gtk_widget_show_all (objectsGrid);
    
}

void removeButton_clicked_cb(GtkButton *b){
    if(rows != 0){
        gtk_grid_remove_row(objectsGrid, rows);
        gtk_widget_show_all (objectsGrid);
        rows--;
    }
}

void executeButton_clicked_cb(GtkButton *b){
    double **matrix = getData();
    
    /*for(int i = 1; i < rows; i++){
        for(int j = 0; j < 3; j++){
            printf("Valor %f  ", matrix[i][j]);
        }
        printf("\n");
    }*/
      // printf("Oh %f\n", matrix[0][0]);
    runAlgorithms(matrix);
    free(matrix);

}   


/*********************************
 *                               *
 *                               *
 *      INITIALIZE OBJECTS       *
 *                               *
 *********************************/

void initializeDynamic(){
    dynamicScroll = GTK_WIDGET(gtk_builder_get_object(builder, "dynamicScroll"));
    dynamicGrid = GTK_WIDGET(gtk_builder_get_object(builder, "dynamicGrid"));
}
void intializeObjects(){

    GdkColor color;
    gdk_color_parse("#24ca86", &color);

    objectsScroll = GTK_WIDGET(gtk_builder_get_object(builder, "objectsScroll"));
    objectsGrid = GTK_WIDGET(gtk_builder_get_object(builder, "objectsGrid"));
}

void initializeWindow(){
    builder = gtk_builder_new_from_file("glade/mainWIndow.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));

    addButton = GTK_WIDGET(gtk_builder_get_object(builder, "addButton"));

    removeButton = GTK_WIDGET(gtk_builder_get_object(builder, "removeButton"));

    saveFileButton = GTK_WIDGET(gtk_builder_get_object(builder, "saveFileButton"));
   
    chooseFileButton = GTK_WIDGET(gtk_builder_get_object(builder, "chooseFileButton"));

    cleanButton = GTK_WIDGET(gtk_builder_get_object(builder, "cleanButton"));

    executeButton = GTK_WIDGET(gtk_builder_get_object(builder, "executeButton"));

    

    GdkColor color;
    gdk_color_parse("#FFFFFF", &color);

    gtk_widget_modify_bg(GTK_WIDGET(window), GTK_STATE_NORMAL, &color);

}


int main(int argc, char *argv[]){

    gtk_init(&argc, &argv);

    initializeWindow();
    intializeObjects();
    initializeDynamic();

    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_object_unref(builder);
    gtk_widget_show(window);

    gtk_main();

    return 0;
}