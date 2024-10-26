#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 8 // Define tamaño del tablero, constante global

// Estructura del nodo (representa cada casilla del tablero)
typedef struct Node {
    char pieza;  // Tipo de pieza protagonista
    struct Node* norte;
    struct Node* sur;
    struct Node* este;
    struct Node* oeste;
} Node;

// Funcion para crear un nuevo nodo
Node* crearNodo(char pieza) {
    Node* nuevoNodo = (Node*)malloc(sizeof(Node));
    if (nuevoNodo == NULL) {
        printf("Error al asignar memoria.\n");
        exit(1);
    }
    nuevoNodo->pieza = pieza;
    nuevoNodo->norte = nuevoNodo->sur = nuevoNodo->este = nuevoNodo->oeste = NULL;
    return nuevoNodo;
}

// Funcion para convertir el tablero a una malla de nodos
Node*** generarTablero(char tablero[SIZE][SIZE]) {
    Node*** malla = (Node***)malloc(SIZE * sizeof(Node**));
    for (int i = 0; i < SIZE; i++) {
        malla[i] = (Node**)malloc(SIZE * sizeof(Node*));
        for (int j = 0; j < SIZE; j++) {
            malla[i][j] = crearNodo(tablero[i][j]);
        }
    }

    // Enlazar los nodos para formar la malla
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i > 0) malla[i][j]->norte = malla[i-1][j];
            if (i < SIZE - 1) malla[i][j]->sur = malla[i+1][j];
            if (j > 0) malla[i][j]->oeste = malla[i][j-1];
            if (j < SIZE - 1) malla[i][j]->este = malla[i][j+1];
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

// Funcion para leer el archivo y llenar el tablero 
void leerArchivo(char* nombreArchivo, char tablero[SIZE][SIZE], char* coordenada) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        exit(1);
    }

    // Leer las primeras 8 lineas
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fscanf(archivo, " %c", &tablero[i][j]);
        }
    }
    
    // Leer la coordenada de la pieza protagonista
    fscanf(archivo, "%s", coordenada);

    fclose(archivo);
}

// funcion para obtener el valor de una pieza
int obtenerValorPieza(char pieza) {
    switch(pieza) {
        case 'P': return 1;
        case 'A': return 3;
        case 'C': return 3;
        case 'T': return 5;
        case 'Q': return 8;
        case 'K': return 10;
        default: return 0;
    }
}

// funcion para convertir notacion de ajedrez a índices de matriz
void convertirCoordenada(char* coordenada, int* fila, int* columna) {
    *columna = coordenada[0] - 'A';
    *fila = 8 - (coordenada[1] - '0');
}

// funcion para encontrar y mostrar los movimientos validos de la torre
void movimientosTorre(Node *** malla, int fila, int columna){
    printf("Movimientos válidos: \n");

    // movimiento al norte
    for(int i = fila -1 ; i >= 0; i--){
        char pieza = malla[i][columna]->pieza;
        printf("%c%d - %d\n", 'A' + columna, 8 - i, obtenerValorPieza(pieza));
        if(pieza !='.') break;
    }
    
    // movimiento al sur
    for(int i = fila +1 ; i < SIZE; i++){
        char pieza = malla[i][columna]->pieza;
        printf("%c%d - %d\n", 'A' + columna, 8 - i, obtenerValorPieza(pieza));
        if(pieza !='.') break;
    }

    // movimiento al este
    for(int j = columna +1 ; j < SIZE; j++){
        char pieza = malla[fila][j]->pieza;
        printf("%c%d - %d\n", 'A' + j, 8 - fila, obtenerValorPieza(pieza));
        if(pieza !='.') break;
    }

    // movimiento al oeste
    for(int j = columna -1 ; j >= 0; j--){
        char pieza = malla[fila][j]->pieza;
        printf("%c%d - %d\n", 'A' + j, 8 - fila, obtenerValorPieza(pieza));
        if(pieza !='.') break;
    }
} 

// función para encontrar y mostrar los movimientos válidos del alfil
void movimientosAlfil(Node*** malla, int fila, int columna) {
    printf("Movimientos válidos para el alfil:\n");

    // Movimiento noreste
    for (int i = fila - 1, j = columna + 1; i >= 0 && j < SIZE; i--, j++) {
        char pieza = malla[i][j]->pieza;
        printf("%c%d - %d\n", 'A' + j, 8 - i, obtenerValorPieza(pieza));
        if (pieza != '.') break;
    }

    // Movimiento sureste
    for (int i = fila + 1, j = columna + 1; i < SIZE && j < SIZE; i++, j++) {
        char pieza = malla[i][j]->pieza;
        printf("%c%d - %d\n", 'A' + j, 8 - i, obtenerValorPieza(pieza));
        if (pieza != '.') break;
    }

    // Movimiento suroeste
    for (int i = fila + 1, j = columna - 1; i < SIZE && j >= 0; i++, j--) {
        char pieza = malla[i][j]->pieza;
        printf("%c%d - %d\n", 'A' + j, 8 - i, obtenerValorPieza(pieza));
        if (pieza != '.') break;
    }

    // Movimiento noroeste
    for (int i = fila - 1, j = columna - 1; i >= 0 && j >= 0; i--, j--) {
        char pieza = malla[i][j]->pieza;
        printf("%c%d - %d\n", 'A' + j, 8 - i, obtenerValorPieza(pieza));
        if (pieza != '.') break;
    }
}


int main() {
    char nombreArchivo[100];
    char tablero[SIZE][SIZE];
    char coordenada[3];  // almacenar la coordenada de la pieza protagonista

    // Solicitar el nombre del archivo de entrada
    printf("Ingrese el nombre del archivo de entrada con extensión .txt: ");
    scanf("%s", nombreArchivo);

    // Llama a la función leerArchivo y manda la variable nombreArchivo y tablero 8x8
    leerArchivo(nombreArchivo, tablero, coordenada);

    // Generar la malla del tablero
    Node*** malla = generarTablero(tablero); // Triple puntero llamado "malla"

    // Imprimir el tablero
    printf("Tablero generado:\n");
    imprimirTablero(malla);

    // convertir la coordenada a índice de matriz
    int fila, columna;
    convertirCoordenada(coordenada, &fila, &columna);

    // Verificar el tipo de pieza protagonista y llamar a la función correspondiente
    char piezaProtagonista = malla[fila][columna]->pieza;
    printf("\nAnalizando movimientos para la pieza en %s:\n", coordenada);

        if (piezaProtagonista == 'T') {
            printf("Pieza protagonista: Torre\n");
            movimientosTorre(malla, fila, columna);
        } else if (piezaProtagonista == 'A') {
            printf("Pieza protagonista: Alfil\n");
            movimientosAlfil(malla, fila, columna);
        } else {
            printf("Pieza no válida para este análisis. Solo se permiten Torres (T) y Alfiles (A).\n");
        }

    // Liberar memoria
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            free(malla[i][j]);
        }
        free(malla[i]);
    }
    free(malla);

    return 0;
}
