#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int rows = 1;
int cols = 3;
int knapsackSize = 1;
int ceroUno = 1;

GtkWidget *window;
GtkBuilder *builder;
GtkWidget *objectsScroll;
GtkWidget *dynamicScroll;
GtkWidget *greedy1Scroll;
GtkWidget *greedy2Scroll;
GtkWidget *objectsGrid;
GtkWidget *dynamicGrid;
GtkWidget *gridGreedy1;
GtkWidget *gridGreedy2;

GtkWidget *addButton;
GtkWidget *removeButton;
GtkWidget *saveFileButton;
GtkWidget *chooseFileButton;

GtkWidget *cleanButton;
GtkWidget *executeButton;

GtkWidget *sizeSpinButton;

GtkWidget *fileC;
GtkWidget *fileS;

GtkWidget *maxLabel;
GtkWidget *formulaLabel;
GtkWidget *sujLabel;
GtkWidget *sujetoLabel;

extern void loadLabel(char *str, GtkWidget *grid, GdkColor bg, GdkColor fg, int i, int j, int width, int height);
extern void greedy1(struct Objeto *objetos, int size, int pesoMax);
extern void greedy2(struct Objeto *objetos, int size, int pesoMax);
extern void dinamic(struct Objeto *objetos, int productos, int pesoMax);

struct Objeto
{
    float valor;
    int costo;
    int cantidad;
    float relacion;
};

/*********************************
 *                               *
 *                               *
 *             ERRORS            *
 *                               *
 *********************************/

void showDialog(char *msg)
{
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *label;

    gint response_id;

    dialog = gtk_dialog_new_with_buttons("!!!!",
                                         window,
                                         GTK_DIALOG_MODAL,
                                         "Ok",
                                         GTK_RESPONSE_OK,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    label = gtk_label_new(msg);
    gtk_container_add(GTK_CONTAINER(content_area), label);

    gtk_widget_show_all(dialog);

    g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(gtk_widget_destroy), NULL);
}

/*********************************
 *                               *
 *                               *
 *              DATA             *
 *                               *
 *********************************/

void clean()
{
    for (int i = rows - 1; i > 0; i--)
    {
        gtk_grid_remove_row(objectsGrid, i);
    }
    for (int i = knapsackSize; i >= 0; i--)
    {
        gtk_grid_remove_column(dynamicGrid, i);
    }

    for (int i = 3; i >= 0; i--)
    {
        gtk_grid_remove_row(gridGreedy1, i);
        gtk_grid_remove_row(gridGreedy2, i);
    }

    gtk_spin_button_set_value(sizeSpinButton, 0);

    gtk_widget_show_all(objectsGrid);
    gtk_widget_show_all(dynamicGrid);
    gtk_widget_show_all(gridGreedy1);
    gtk_widget_show_all(gridGreedy2);
    gtk_widget_set_sensitive(executeButton, FALSE);
    rows = 1;
    knapsackSize = 1;
}

struct Objeto *copyArray(struct Objeto *object, int size)
{
    struct Objeto *result = malloc(rows * 50);
    for (int i = 0; i < size; i++)
    {
        result[i] = object[i];
    }
    return result;
}

void loadGreedy1Equation(struct Objeto *objetos, GtkWidget *grid)
{
    GdkColor colorBg;
    GdkColor colorFg;
    gdk_color_parse("#F34614", &colorBg);
    gdk_color_parse("white", &colorFg);

    

    char temp[rows * 100];
    char equation[rows * 100];
    memset(equation, 0, rows * 2);

    //Monta la forma matemática del algoritmo
    strcpy(equation, "Z = ");
    for (int i = 0; i < rows - 1; i++)
    {
        memset(temp, 0, rows * 100);

        sprintf(temp, "%.3f", objetos[i].valor);

        strcat(equation, temp);
        strcat(equation, "x");

        memset(temp, 0, rows * 100);
        sprintf(temp, "%d", i);
        strcat(equation, temp);

        if (i != rows - 2)
        {
            strcat(equation, " +");
        }
    }

    GtkWidget *label;

    label = gtk_label_new(equation);
    gtk_widget_modify_bg(GTK_WIDGET(label), GTK_STATE_NORMAL, &colorBg);
    gtk_widget_modify_fg(GTK_WIDGET(label), GTK_STATE_NORMAL, &colorFg);
    gtk_widget_set_size_request(label, 320 + rows * 2, 40);
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    gtk_widget_show_all(grid);
}

double **getData()
{
    double **resultData = (double **)malloc(sizeof(double *) * (rows + 1) * cols);
    GtkSpinButton *spin;
    fflush(stdout);
    for (int i = 1; i < rows; i++)
    {
        resultData[i - 1] = (double *)malloc(sizeof(double *) * cols);
        for (int j = 0; j < cols; j++)
        {
            spin = GTK_SPIN_BUTTON(gtk_grid_get_child_at(objectsGrid, j, i));
            resultData[i - 1][j] = gtk_spin_button_get_value(spin);
        }
    }
    return resultData;
}

void runAlgorithms(double **matrix)
{
    char restriction1[rows * 100];
    char restriction2[rows * 100];
    char temp2[rows * 100];
    char temp[rows * 100];
    strcpy(restriction1, " ");
    strcpy(restriction2, "xi ≥ 0");
    char equation[rows * 100];
    memset(equation, 0, rows * 2);
    strcpy(equation, "Z = ");

    GdkColor colorBg;
    GdkColor colorFg;
    gdk_color_parse("#BA11FF", &colorBg);
    gdk_color_parse("white", &colorFg);
    int size = (rows)*50;
    struct Objeto *elements = malloc(size);
    struct Objeto objeto;
    GtkWidget *label;
    label = gtk_label_new("");

    gtk_widget_modify_bg(GTK_WIDGET(label), GTK_STATE_NORMAL, &colorBg);
    gtk_widget_modify_fg(GTK_WIDGET(label), GTK_STATE_NORMAL, &colorFg);
    gtk_widget_set_size_request(label, 50, 40);
    gtk_grid_attach(GTK_GRID(dynamicGrid), label, 0, 0, 1, 1);

    for (int i = 0; i < rows - 1; i++)
    {

        objeto.costo = matrix[i][0];
        objeto.valor = matrix[i][1];
        if (matrix[i][2] == 0)
        {
            showDialog("Quantity cannot be 0 :0");
            return;
        }
        else if (matrix[i][2] == -1)
        {
            objeto.cantidad = 2147483647;
            ceroUno = 0;
        }
        else
        {
            if (matrix[i][2] != 1)
            {
                ceroUno = 0;
            }
            objeto.cantidad = (int)matrix[i][2];
        }
        objeto.relacion = ((float)objeto.valor / (float)objeto.costo);
        elements[i] = objeto;

        char str[48];

        char temp3[48];
        memset(temp, 0, rows * 100);
        sprintf(temp, "%.3f", objeto.valor);
        strcat(equation, temp);
        strcat(equation, "x");
        memset(temp, 0, rows * 100);
        sprintf(temp, "%d", i);
        strcat(equation, temp);

        
        memset(temp2, 0, rows * 100);
        sprintf(temp2, "%d", objeto.costo);
        strcat(restriction1, temp2);
        strcat(restriction1, "x");
        strcat(restriction1, temp);
        if (objeto.cantidad == 1)
        {
            strcat(restriction2, ", x");
            strcat(restriction2, temp);
            strcat(restriction2, "=0/1 ");
        }
        else if (objeto.cantidad != 2147483647)
        {
            strcat(restriction2, ", x");
            strcat(restriction2, temp);
            strcat(restriction2, "≤");
            char cant[5];
            sprintf(cant, "%d", objeto.cantidad);
            strcat(restriction2, cant);
        }
        if (i != rows - 2)
        {
            strcat(restriction1, " +");
            strcat(equation, " +");
        }
        //carga las filas y columnas del algoritmo
        strcpy(str, "x");
        sprintf(temp3, "%d", i);
        strcat(str, temp3);
        loadLabel(str, dynamicGrid, colorBg, colorFg, 0, i + 1, 1, 40);

        loadLabel(temp3, dynamicGrid, colorBg, colorFg, i + 1, 0, 1, 40);
    }
    strcat(restriction1, " ≤ ");
    char mochila[5];
    sprintf(mochila, "%d\n", knapsackSize);
    strcat(restriction1, mochila);
    if (ceroUno)
        strcat(restriction1, " xi = 0 o 1");
    else
        strcat(restriction1, restriction2);
    gtk_label_set_text(formulaLabel, equation);
    gtk_label_set_text(sujetoLabel, restriction1);
    gtk_widget_show(formulaLabel);
    struct Objeto *elements2 = copyArray(elements, rows - 1);
    struct Objeto *elements3 = copyArray(elements, rows - 1);

    dinamic(elements, (rows - 1), knapsackSize);

    greedy1(elements2, (rows - 1), knapsackSize);

    greedy2(elements3, (rows - 1), knapsackSize);
    free(elements);
}

/*********************************
 *                               *
 *                               *
 *           GTK EVENTS          *
 *                               *
 *********************************/

void on_window_main_destroy()
{
    gtk_main_quit();
}

void addButton_clicked_cb(GtkButton *b)
{
    gtk_widget_set_sensitive(executeButton, TRUE);
    gtk_grid_attach(GTK_GRID(objectsGrid), gtk_spin_button_new(gtk_adjustment_new(0, 1, 2147483647, 1, 1, 1), 1, 0), 0, rows, 1, 1);
    gtk_grid_attach(GTK_GRID(objectsGrid), gtk_spin_button_new(gtk_adjustment_new(0, 1, 2147483647, 1, 1, 1), 1, 3), 1, rows, 1, 1);
    gtk_grid_attach(GTK_GRID(objectsGrid), gtk_spin_button_new(gtk_adjustment_new(1, -1, 2147483647, 1, 1, 1), 1, 0), 2, rows, 1, 1);
    rows++;
    gtk_widget_show_all(objectsGrid);
}

void removeButton_clicked_cb(GtkButton *b)
{
    if (rows > 1)
    {
        gtk_grid_remove_row(objectsGrid, rows - 1);
        gtk_widget_show_all(objectsGrid);
        rows--;
        if(rows <= 1) gtk_widget_set_sensitive(executeButton, FALSE);
    }
}

void executeButton_clicked_cb(GtkButton *b)
{
    for (int i = rows - 1; i >= 0; i--)
    {
        gtk_grid_remove_column(dynamicGrid, i);
    }

    for (int i = 3; i >= 0; i--)
    {
        gtk_grid_remove_row(gridGreedy1, i);
        gtk_grid_remove_row(gridGreedy2, i);
    }
    knapsackSize = gtk_spin_button_get_value_as_int(sizeSpinButton);
    double **matrix = getData();
    gtk_widget_show_all(window);
    runAlgorithms(matrix);
    free(matrix);
}

void cleanButton_clicked_cb(GtkButton *b)
{
    clean();
}

void chooseFileButton_clicked_cb(GtkButton *b)
{
    gtk_widget_show(fileC);
}

void saveFileButton_clicked_cb(GtkButton *b)
{
    gtk_widget_show(fileS);
}

void cancelChoose_clicked_cb(GtkButton *b)
{
    gtk_widget_hide(fileC);
}

void cancelSave_clicked_cb(GtkButton *b)
{
    gtk_widget_hide(fileS);
}

void openFile_clicked_cb(GtkButton *b)
{
    gchar *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileC));
    if (strstr(path, ".sack") != NULL)
    {
        clean();
        FILE *fp;
        fp = fopen(path, "r");
        char buffer[255];
        fscanf(fp, "%d %d ", &knapsackSize, &rows);
        double matriz[rows - 1][cols];
        for (int i = 1; i < rows; i++)
        {
            fscanf(fp, "%lf %lf %lf\n", &matriz[i][0], &matriz[i][1], &matriz[i][2]);
            gtk_grid_attach(GTK_GRID(objectsGrid), gtk_spin_button_new(gtk_adjustment_new(matriz[i][0], 1, 2147483647, 1, 1, 1), 1, 0), 0, i, 1, 1);
            gtk_grid_attach(GTK_GRID(objectsGrid), gtk_spin_button_new(gtk_adjustment_new(matriz[i][1], 1, 2147483647, 1, 1, 1), 1, 3), 1, i, 1, 1);
            gtk_grid_attach(GTK_GRID(objectsGrid), gtk_spin_button_new(gtk_adjustment_new(matriz[i][2], -1, 2147483647, 1, 1, 1), 1, 0), 2, i, 1, 1);
        }
        gtk_spin_button_set_value(sizeSpinButton, knapsackSize);
        //gtk_widget_show_all(sizeSpinButton);
        gtk_widget_show_all(objectsGrid);
        fclose(fp);
        gtk_widget_set_sensitive(executeButton, TRUE);
        gtk_widget_hide(fileC);
    }
    else
    {
        showDialog("Invalid file extension");
    }
}

void saveFile_clicked_cb(GtkButton *b)
{
    gchar *name = gtk_file_chooser_get_current_name(GTK_FILE_CHOOSER(fileS));
    if (!strcmp(name, ""))
    {
        showDialog("Type a name");
        return;
    }
    gchar *path = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(fileS));
    strcat(path, "/");
    strcat(path, name);
    strcat(path, ".sack");
    FILE *fp;
    fp = fopen(path, "w+");
    char data[255];
    knapsackSize = gtk_spin_button_get_value_as_int(sizeSpinButton);
    sprintf(data, "%d %d ", knapsackSize, rows);
    fputs(data, fp);
    double **matrix = getData();
    for (int i = 0; i < rows - 1; i++)
    {
        sprintf(data, "%f %f %f ", matrix[i][0], matrix[i][1], matrix[i][2]);
        fputs(data, fp);
    }
    free(matrix);
    fclose(fp);
    gtk_widget_hide(fileS);
}

/*********************************
 *                               *
 *                               *
 *      INITIALIZE OBJECTS       *
 *                               *
 *********************************/

void initializeGreedies()
{
    greedy1Scroll = GTK_WIDGET(gtk_builder_get_object(builder, "greedy1Scroll"));
    gridGreedy1 = GTK_WIDGET(gtk_builder_get_object(builder, "gridGreedy1"));

    greedy2Scroll = GTK_WIDGET(gtk_builder_get_object(builder, "greedy2Scroll"));
    gridGreedy2 = GTK_WIDGET(gtk_builder_get_object(builder, "gridGreedy2"));
}

void initializeDynamic()
{
    dynamicScroll = GTK_WIDGET(gtk_builder_get_object(builder, "dynamicScroll"));
    dynamicGrid = GTK_WIDGET(gtk_builder_get_object(builder, "dynamicGrid"));
}
void intializeObjects()
{

    GdkColor color;
    gdk_color_parse("#24ca86", &color);

    objectsScroll = GTK_WIDGET(gtk_builder_get_object(builder, "objectsScroll"));
    objectsGrid = GTK_WIDGET(gtk_builder_get_object(builder, "objectsGrid"));
}

void initializeWindow()
{
    builder = gtk_builder_new_from_file("glade/mainWIndow.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));

    addButton = GTK_WIDGET(gtk_builder_get_object(builder, "addButton"));

    removeButton = GTK_WIDGET(gtk_builder_get_object(builder, "removeButton"));

    saveFileButton = GTK_WIDGET(gtk_builder_get_object(builder, "saveFileButton"));

    chooseFileButton = GTK_WIDGET(gtk_builder_get_object(builder, "chooseFileButton"));

    cleanButton = GTK_WIDGET(gtk_builder_get_object(builder, "cleanButton"));

    executeButton = GTK_WIDGET(gtk_builder_get_object(builder, "executeButton"));

    sizeSpinButton = GTK_WIDGET(gtk_builder_get_object(builder, "sizeSpinButton"));

    gtk_spin_button_set_adjustment(sizeSpinButton, gtk_adjustment_new(0, 1, 2147483647, 1, 1, 1));

    fileC = GTK_WIDGET(gtk_builder_get_object(builder, "choose"));
    fileS = GTK_WIDGET(gtk_builder_get_object(builder, "save"));

    maxLabel = GTK_WIDGET(gtk_builder_get_object(builder, "max"));
    formulaLabel = GTK_WIDGET(gtk_builder_get_object(builder, "formulaLabel"));
    sujLabel = GTK_WIDGET(gtk_builder_get_object(builder, "suj"));
    sujetoLabel = GTK_WIDGET(gtk_builder_get_object(builder, "sujetoLabel"));

    GdkColor color;
    gdk_color_parse("#FFFFFF", &color);

    gtk_widget_modify_bg(GTK_WIDGET(window), GTK_STATE_NORMAL, &color);
}

int main(int argc, char *argv[])
{

    gtk_init(&argc, &argv);

    initializeWindow();
    intializeObjects();
    initializeDynamic();
    initializeGreedies();

    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_object_unref(builder);
    gtk_widget_show(window);

    gtk_main();

    return 0;
}