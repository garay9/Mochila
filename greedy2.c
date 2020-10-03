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
    int pesoActual = 0;
    int contadorCantidad = 0;
    quickSort(objetos, 0, size-1);

    for (int i = 0; i < size && pesoActual <= pesoMax; i++)
    {
        printf("%s :", objetos[i].nombre);
        while (objetos[i].cantidad > 0 && pesoActual + objetos[i].costo <= pesoMax)
        {
            objetos[i].cantidad -= 1;
            contadorCantidad += 1;
            pesoActual += objetos[i].costo;
        }
        printf("%d usados\n", contadorCantidad);
        contadorCantidad = 0;
    }
    printf("Peso total usado: %d\n", pesoActual);
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
    anillo.cantidad = 3;
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
}