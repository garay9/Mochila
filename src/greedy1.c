#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Objeto
{
    int valor;
    int costo;
    int cantidad;
    float relacion;
    char nombre[256];
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
        if (arr[j].valor > pivot)
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

void greedy1(struct Objeto *objetos, int size, int pesoMax){
    int valorTotal = 0;
    int pesoActual = 0;
    int cantidadObjeto = 0;
    quickSort(objetos, 0, size-1);

    for (int i = 0; i < size && pesoActual <= pesoMax; i++)
    {
        printf("Objeto: %s\n", objetos[i].nombre);
        while (objetos[i].cantidad != 0 && pesoActual + objetos[i].costo <= pesoMax)
        {
            objetos[i].cantidad -= 1;  
            cantidadObjeto += 1; 
            valorTotal += objetos[i].valor;
            pesoActual += objetos[i].costo;
        }
        printf("Cantidad recolectada: %i\n", cantidadObjeto);
        cantidadObjeto = 0;
    }
    printf("Peso usado: %d\n", pesoActual);
    printf("Valor conseguido: %d\n", valorTotal);
}


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
}