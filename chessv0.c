#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 8 //define tamano del tablero, constante global

// Estructura del nodo (representa cada casilla del tablero)
typedef struct Node {
    char pieza;  // Tipo de pieza protagonista
    struct Node* norte;
    struct Node* sur;
    struct Node* este;
    struct Node* oeste;
} Node;

// Función para crear un nuevo nodo
Node* crearNodo(char pieza) { // Devuelve un puntero a un nodo (Node*), que sera utilizado para crear una casilla del tablero.
    Node* nuevoNodo = (Node*)malloc(sizeof(Node)); // Crea puntero local y determina el espacio en memoria que usara
    if (nuevoNodo == NULL) { // Comprobar si la asignacion de memoria fallo
        printf("Error al asignar memoria.\n");
        exit(1);
    }
    nuevoNodo->pieza = pieza; // Se asigna el valor del char "pieza" a la pieza del nuevo nodo
    nuevoNodo->norte = nuevoNodo->sur = nuevoNodo->este = nuevoNodo->oeste = NULL; // Inicializamos cada Nodo a las cuatro posibles direcciones a NULL (aun no esta conectado)
    return nuevoNodo;
}

// Función para convertir el tablero a una malla de nodos, devuelve puntero a una malla de nodos
Node*** generarTablero(char tablero[SIZE][SIZE]) { // Recibe el tablero ya lleno 
    Node*** malla = (Node***)malloc(SIZE * sizeof(Node**)); // Crea triple puntero local llamado "malla", y determina el espacio en memoria que usara
    for (int i = 0; i < SIZE; i++) { 
        malla[i] = (Node**)malloc(SIZE * sizeof(Node*)); // Para cada elemento en cada fila en malla, alocar memoria para 8 punteros
        for (int j = 0; j < SIZE; j++) { // El ciclo for anterior creo las columnas dinamicas. Ahora, en cada elemento de la "matriz" llenaremos de Nodos
            malla[i][j] = crearNodo(tablero[i][j]); //
        }
    }

    // Enlazar los nodos para formar la malla
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i > 0) malla[i][j]->norte = malla[i-1][j]; //Si el nodo NO está en la primera fila, enlazamos el nodo actual (malla[i][j]) con el nodo al norte
            if (i < SIZE - 1) malla[i][j]->sur = malla[i+1][j]; //Si el nodo NO está en la última fila, enlazamos el nodo actual con el nodo al sur
            if (j > 0) malla[i][j]->oeste = malla[i][j-1]; // Si el nodo NO está en la primera columna, enlazamos el nodo actual con el nodo al oeste
            if (j < SIZE - 1) malla[i][j]->este = malla[i][j+1]; //Si el nodo NO está en la última columna, enlazamos el nodo actual con el nodo al este
        }
    }

    return malla;
}

// Función para imprimir la malla
void imprimirTablero(Node*** malla) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", malla[i][j]->pieza);
        }
        printf("\n");
    }
}

// Función para leer el archivo y llenar el tablero
void leerArchivo(char* nombreArchivo, char tablero[SIZE][SIZE]) { // Recibe puntero a char y el tablero8x8 (vacio)
    FILE* archivo = fopen(nombreArchivo, "r"); // Abre el archivo y lo guarda en el puntero (de tipo file) llamado archivo
    if (archivo == NULL) { // Si el puntero esta vacio, terminar el programa
        printf("Error al abrir el archivo.\n");
        exit(1);
    }

    // Leer LAS PRIMERAS 8 LINEAS del archivo, probablemente tener que aumentar en Etapa 2
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fscanf(archivo, " %c", &tablero[i][j]); // Llenar lo leido en cada espacio de tablero
        }
    }

    fclose(archivo);
}

int main() {
    char nombreArchivo[100];
    char tablero[SIZE][SIZE]; // Matriz de char 8x8

    // Solicitar el nombre del archivo de entrada
    printf("Ingrese el nombre del archivo de entrada con extension .txt: ");
    scanf("%s", nombreArchivo); // Guarda el nombre del archivo de entrada en la variable nombreArchivo

    // Llama a la funcion leerArchivo y manda la variable nombreArchivo y tablero8x8
    leerArchivo(nombreArchivo, tablero);

    // Generar la malla del tablero
    Node*** malla = generarTablero(tablero); //triple puntero llamado "malla", es igual al output de la funcion generarTablero

    // Imprimir el tablero
    printf("Tablero generado:\n");
    imprimirTablero(malla); //Llama a la funcion imprimirTablero y manda el triple puntero "malla"

    // Liberar la memoria utilizada
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            free(malla[i][j]);
        }
        free(malla[i]);
    }
    free(malla);

    return 0;

    typedef struct {
        int fila;
        int columna;
        int valor;
    } mov;

    void convCoord(char* coord, int* fila, int* columna) {
    *columna = toupper(coord[0]) - 'A';
    *fila = 8 - (coord[1] - '0');
    }

    int valorPieza(char pieza){
        switch(pieza){
            case 'P': return 1;
             case 'A': case 'C': return 3;
            case 'T': return 5;
            case 'Q': return 8;
            case 'K': return 10;
            default: return 0;
         }   
        }
}