#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <math.h>

extern GtkWidget *gridGreedy1;
extern GtkWidget *gridGreedy2;

extern void loadGreedy1Equation(struct Objeto *objetos, GtkWidget *grid);

struct Objeto
{
    float valor;
    int costo;
    int cantidad;
    float relacion;
};

void loadLabel(char *str, GtkWidget *grid, GdkColor bg, GdkColor fg, int i, int j, int width, int height){
        GtkWidget *label;

        label =  gtk_label_new (str); 
        gtk_widget_modify_bg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &bg);
        gtk_widget_modify_fg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &fg);
        gtk_widget_set_size_request(label, width*50, 40);
        gtk_grid_attach (GTK_GRID(grid), label, j, i,1,1);
         gtk_widget_show_all(grid);

}


void invertir(struct Objeto *objetos, int size){
    struct Objeto temp;
    int newSize = size/ 2;
     for (int i = 0; i < newSize ; i++) {
   
    temp = objetos[i];
    
    objetos[i] = objetos[newSize - i - 1];
    
    objetos[newSize - i - 1] = temp;
  }

}

void swapA(struct Objeto *a, struct Objeto *b)
{
    struct Objeto t = *a;
    *a = *b;
    *b = t;
}

int partitionA(struct Objeto *arr, int low, int high)
{
    float pivot = arr[high].valor;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j].valor > pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swapA(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSortA(struct Objeto *arr, int low, int high)
{
    if (low < high)
    {
        int pi = partitionA(arr, low, high);
        quickSortA(arr, low, pi - 1);
        quickSortA(arr, pi + 1, high);
    }
}

void greedy1(struct Objeto *objetos, int size, int pesoMax){
    GdkColor colorBg;
    GdkColor colorFg;
    gdk_color_parse ("#E68110", &colorBg);
    gdk_color_parse ("white", &colorFg);

    float valorTotal = 0;
    int pesoActual = 0;
    int cantidadObjeto = 0;
    quickSortA(objetos, 0, size-1);
    invertir(objetos, size - 1);
    loadGreedy1Equation(objetos, gridGreedy1);
    char variableResult[pesoMax*2+1];
    char temp[pesoMax+1];
    char labelResult[sizeof(variableResult)* size];
    memset(labelResult, 0, sizeof(variableResult)* size);
    
    for (int i = 0; i < size && pesoActual <= pesoMax; i++)
    {

        while (objetos[i].cantidad != 0 && pesoActual + objetos[i].costo <= pesoMax)
        {
            objetos[i].cantidad -= 1;  
            cantidadObjeto += 1; 
            valorTotal += objetos[i].valor;
            pesoActual += objetos[i].costo;
        }
       
       //coloca la cantidad óptima para cada variable
        strcpy(variableResult,"x");
        memset(temp, 0, pesoMax+1);
        sprintf(temp, "%d", i);
        strcat(variableResult, temp);
        strcat(variableResult, " = ");
        memset(temp, 0, pesoMax+1);
        sprintf(temp, "%d", cantidadObjeto);
        strcat(variableResult, temp);
        strcat(variableResult, " ");

       

        strcat(labelResult, variableResult);
        cantidadObjeto = 0;
    }

       //carga el resultado de las variables

       loadLabel(labelResult, gridGreedy1, colorBg, colorFg, 1, 0, size, 40);
        memset(temp, 0, size+1);
        sprintf(temp, "%f", valorTotal);
        gdk_color_parse ("#E6C610", &colorBg);

        //coloca el Z máximo
        loadLabel(temp, gridGreedy1 , colorBg, colorFg, 2, 0, size, 40);

}

/*
int main(int argc, char *argv[])
{
    struct Objeto *elementos = malloc(2048);
    struct Objeto anillo;
    struct Objeto poster;
    struct Objeto radio;
    struct Objeto candelabro;

    anillo.valor = 15000;
    anillo.costo = 1;
    anillo.cantidad = 1;
    anillo.relacion = (float)anillo.valor / (float)anillo.costo;
    strcpy(anillo.nombre, "Anillo");

    poster.valor = 5000;
    poster.costo = 4;
    poster.cantidad = 6;
    poster.relacion = (float)poster.valor / (float)poster.costo;
    strcpy(poster.nombre, "Poster");

    radio.valor = 9000;
    radio.costo = 3;
    radio.cantidad = 5;
    radio.relacion = (float)radio.valor / (float)radio.costo;
    strcpy(radio.nombre, "Radio");

    candelabro.valor = 10000;
    candelabro.costo = 5;
    candelabro.cantidad = 3;
    candelabro.relacion = (float)candelabro.valor / (float)candelabro.costo;
    strcpy(candelabro.nombre, "Candelabro");

    elementos[0] = anillo;
    elementos[1] = poster;
    elementos[2] = radio;
    elementos[3] = candelabro;
    greedy1(elementos,4,60);
    

    return 0;
}*/