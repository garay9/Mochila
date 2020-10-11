#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

struct Objeto
{
    float valor;
    int costo;
    int cantidad;
    float relacion;
    char nombre[256];
};

extern GtkWidget *dynamicGrid; 
extern int rows;
extern int cols;
void dinamic(struct Objeto *objetos, int productos, int pesoMax);

void dinamic(struct Objeto *objetos, int productos, int pesoMax){
    pesoMax++;
    int Table[pesoMax][productos];
    int Llevar[pesoMax][productos];
    GdkColor colorFg;
    GdkColor colorBg;
    gdk_color_parse ("white", &colorFg);
    for (int i = 0; i < pesoMax; i++){
        int cant = i/objetos[0].costo; 
        Table[i][0] = objetos[0].valor * cant;
        Llevar[i][0] = cant;
    }

    for(int j = 1; j < productos; j++){       
        for(int i = 0; i < pesoMax; i++){
            int Q = MIN(objetos[j].cantidad, floor(i/objetos[j].costo));
            int max = Table[i][j-1];
            Llevar[i][j] = 0;
            for(int e = 1; e <= Q; e++){
                int temp = e * objetos[j].valor + Table[i-e * objetos[j].costo][j-1];
                if (temp > max){ 
                    max = temp;
                    Llevar[i][j] = e;
                }

            }
            Table[i][j] = max;
        }       
    }
   
    for(int i = 0; i < pesoMax ; i++){
         
        GtkWidget *label;
        char temp[48];
        sprintf(temp, "%d", i);
        gdk_color_parse ("#BA11FF", &colorBg);
        label =  gtk_label_new (temp);
        gtk_widget_set_size_request(label, 50, 40);
        gtk_widget_modify_bg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &colorBg);
        gtk_widget_modify_fg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &colorFg);
        gtk_grid_attach (GTK_GRID(dynamicGrid), label, 0, i+1,1,1);

        for(int j = 0; j < productos; j++){
            char data[182];
   
            sprintf(data, "%d", Table[i][j]);

            if(Llevar[i][j] == 0) 
                gdk_color_parse ("red", &colorBg);
            else
                gdk_color_parse ("#1188F0", &colorBg);
            
            label  =  gtk_label_new (data);
            gtk_widget_modify_bg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &colorBg);
            gtk_widget_modify_fg ( GTK_WIDGET(label), GTK_STATE_NORMAL, &colorFg);
            gtk_widget_set_size_request(label, 50, 40);
            gtk_grid_attach (GTK_GRID(dynamicGrid), label, j+1, i+1,1,1);
        }

        
    }
    fflush(stdout);
    gtk_widget_show_all(dynamicGrid); 
}

   


/*
int main(int argc, char *argv[]){
    struct Objeto *objetos = malloc(2048);
    struct Objeto galletas;
    struct Objeto medias;
    struct Objeto botella;
    
    galletas.valor = 12;
    galletas.costo = 5;
    galletas.cantidad = 99;

    botella.valor = 11;
    botella.costo = 4;
    botella.cantidad = 99;

    medias.valor = 7;
    medias.costo = 3;
    medias.cantidad = 99;

    objetos[0] = botella;
    objetos[1] = medias;
    objetos[2] = galletas;

    struct Objeto *elementos = malloc(2048);
    struct Objeto anillo;
    struct Objeto poster;
    struct Objeto radio;
    struct Objeto candelabro;

    anillo.valor = 15000;
    anillo.costo = 1;
    anillo.cantidad = 1;

    poster.valor = 5000;
    poster.costo = 4;
    poster.cantidad = 1;

    radio.valor = 9000;
    radio.costo = 3;
    radio.cantidad = 1;

    candelabro.valor = 10000;
    candelabro.costo = 5;
    candelabro.cantidad = 1;

    elementos[0] = candelabro;
    elementos[1] = radio;
    elementos[2] = poster;
    elementos[3] = anillo;
    dinamic(elementos, 4, 8);
    dinamic(objetos, 3, 10);
}
*/