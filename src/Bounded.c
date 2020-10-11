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
};

extern GtkWidget *dynamicGrid; 
extern int rows;
extern int cols;
extern void loadLabel(char *str, GtkWidget *grid, GdkColor bg, GdkColor fg, int i, int j, int width, int height);
void dinamic(struct Objeto *objetos, int productos, int pesoMax);

void dinamic(struct Objeto *objetos, int productos, int pesoMax){
    pesoMax++;
    int Table[pesoMax][productos];
    int Llevar[pesoMax][productos];
    GdkColor colorFg;
    GdkColor colorBg;
    gdk_color_parse ("white", &colorFg);
    int cantidad = 0;
    int peso = 0;
    for (int i = 0; i < pesoMax; i++){
        Table[i][0] = 0;
        while(peso+objetos[0].costo <= i && cantidad+1 <= objetos[0].cantidad){
            peso += objetos[0].costo;
            cantidad++;
            
        }
        Table[i][0] = objetos[0].valor * cantidad;
        if(Table[i][0] != 0){
            Llevar[i][0] = cantidad;
        }else{
            Llevar[i][0] = 0;
        }        
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
        
        //carga el número de fila 
        loadLabel(temp, dynamicGrid, colorBg, colorFg, i+1, 0, 1, 40);

        for(int j = 0; j < productos; j++){
            char data[182];
   
            sprintf(data, "%d", Table[i][j]);

            if(Llevar[i][j] == 0) 
                gdk_color_parse ("red", &colorBg);
            else
                gdk_color_parse ("#1188F0", &colorBg);
            
            //carga las soluciones del algoritmo
            loadLabel(data, dynamicGrid, colorBg, colorFg, i+1, j+1, 1, 40);
        }

        
    }

}

   
