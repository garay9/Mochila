#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <math.h>


extern GtkWidget *gridGreedy2;
extern void loadGreedy1Equation(struct Objeto *objetos, GtkWidget *grid);
extern void loadLabel(char *str, GtkWidget *grid, GdkColor bg, GdkColor fg, int i, int j, int width, int height);
struct Objeto
{
    float valor;
    int costo;
    int cantidad;
    float relacion;
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

    //poner la ecuación del greedy
    loadGreedy1Equation(objetos, gridGreedy2);
    char variableResult[pesoMax*2+1];
    char labelResult[sizeof(variableResult)* size];
    memset(labelResult, 0, sizeof(variableResult)* size);

    for (int i = 0; i < size && pesoActual <= pesoMax; i++)
    {

        while (objetos[i].cantidad != 0 && pesoActual + objetos[i].costo <= pesoMax)
        {
            objetos[i].cantidad -= 1;
            valorTotal += objetos[i].valor;
            contadorCantidad += 1;
            pesoActual += objetos[i].costo;
        }

        //coloca la cantidad óptima para cada variable
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
        //carga el resultado de las variables
        loadLabel(labelResult, gridGreedy2, colorBg, colorFg, 1, 0, size, 40);
        memset(variableResult, 0, pesoMax+1);
        sprintf(variableResult, "%f", valorTotal);
        gdk_color_parse ("#E6C610", &colorBg);
      
        //coloca el Z máximo
        loadLabel(variableResult, gridGreedy2, colorBg, colorFg, 2, 0, size, 40);

   
}