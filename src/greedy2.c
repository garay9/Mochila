#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <math.h>


extern GtkWidget *gridGreedy2;

struct Objeto
{
    float valor;
    int costo;
    int cantidad;
    float relacion;
    char nombre[64];
};

void swap(struct Objeto *a, struct Objeto *b)
{
    struct Objeto t = *a;
    *a = *b;
    *b = t;
}

int partition(struct Objeto *arr, int low, int high)
{
    float pivot = arr[high].relacion;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j].relacion > pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(struct Objeto *arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void greedy2(struct Objeto *objetos, int size, int pesoMax){

    GdkColor colorBg;
    GdkColor colorFg;
    gdk_color_parse ("#E68110", &colorBg);
    gdk_color_parse ("white", &colorFg);

    float valorTotal = 0;
    int pesoActual = 0;
    int contadorCantidad = 0;
    quickSort(objetos, 0, size-1);
    char variableResult[pesoMax*2+1];
    char labelResult[sizeof(variableResult)* size];
     memset(labelResult, 0, sizeof(variableResult)* size);
    for (int i = 0; i < size && pesoActual <= pesoMax; i++)
    {
       // printf("%s :", objetos[i].nombre);
        while (objetos[i].cantidad != 0 && pesoActual + objetos[i].costo <= pesoMax)
        {
            objetos[i].cantidad -= 1;
            valorTotal += objetos[i].valor;
            contadorCantidad += 1;
            pesoActual += objetos[i].costo;
        }
        printf("mmm ok %f\n", valorTotal);
        strcat(labelResult,"x");
        memset(variableResult, 0, pesoMax*2+1);
        sprintf(variableResult, "%d", i);
        strcat(labelResult, variableResult);
        strcat(labelResult, " = ");
        memset(variableResult, 0, pesoMax+1);
        sprintf(variableResult, "%d", contadorCantidad);
        strcat(labelResult, variableResult);
        strcat(labelResult, " ");

      
        contadorCantidad = 0;
    }

        GtkWidget *label;

        label =  gtk_label_new (labelResult); 
        gtk_widget_modify_bg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &colorBg);
        gtk_widget_modify_fg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &colorFg);
        gtk_widget_set_size_request(label, size*50, 40);
        gtk_grid_attach (GTK_GRID(gridGreedy2), label, 0, 1,1,1);
        
        memset(variableResult, 0, pesoMax+1);
        sprintf(variableResult, "%f", valorTotal);
        gdk_color_parse ("#E6C610", &colorBg);
        label =  gtk_label_new (variableResult);
        gtk_widget_modify_fg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &colorFg);
        gtk_widget_modify_bg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &colorBg);
        gtk_grid_attach (GTK_GRID(gridGreedy2), label, 0, 2,1,1);

        gtk_widget_show_all(gridGreedy2); 
    printf("Peso total usado: %d\n", pesoActual);
    printf("Valor conseguido: %d\n", valorTotal);
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
    anillo.cantidad = -1;
    anillo.relacion = (float)anillo.valor / (float)anillo.costo;
    strcpy(anillo.nombre, "Anillo");

    poster.valor = 5000;
    poster.costo = 4;
    poster.cantidad = 1;
    poster.relacion = (float)poster.valor / (float)poster.costo;
    strcpy(poster.nombre, "Poster");

    radio.valor = 9000;
    radio.costo = 3;
    radio.cantidad = 1;
    radio.relacion = (float)radio.valor / (float)radio.costo;
    strcpy(radio.nombre, "Radio");

    candelabro.valor = 10000;
    candelabro.costo = 5;
    candelabro.cantidad = 1;
    candelabro.relacion = (float)candelabro.valor / (float)candelabro.costo;
    strcpy(candelabro.nombre, "Candelabro");

    elementos[0] = anillo;
    elementos[1] = poster;
    elementos[2] = radio;
    elementos[3] = candelabro;
    greedy2(elementos,4,8);
    

    return 0;
}*/