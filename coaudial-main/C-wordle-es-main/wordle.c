/*wordle.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


/* 
    Definicion de valores de resultado segun color
        Como no se crea una variable en memoria,
        no hay costos asociados a la asignación de espacio ni al acceso a esa variable.
    buscar porque 4 y no 3 (Flag)
*/
#define ResultadoVerde 1
#define ResultadoAmarillo 2
#define ResultadoRojo 4
#define MAX_PALABRAS 5504

/* STRUCTS*/
typedef struct {
    char** arr;
    int n;
} Palabras;

/* Renombrando Tipos DE DATOS */
typedef char Resultado;

/* DECLARACION DE FUNCIONES*/
Palabras leerArchivo(const char*);
bool seEncuentraEnPalabra(char, const char*);
void ejemploPrinteoResultados(const char*, const Resultado*);
Resultado* chequeoDePalabra(const char*, const char*);
Resultado chequeoDeCaracter(char, int, const char*);
void liberarPalabras(Palabras);


/* DEFINICÓN DE FUNCIONES*/

/* 
    leerArchivo
    Lee un archivo de texto y carga las palabras de 5 letras en una estructura Palabras.
    - Argumentos:
        - nombreArchivo: Nombre del archivo a leer.
    - Retorno:
        - Un struct Palabras con un arreglo dinámico de palabras y el número total de palabras cargadas.
    - Notas:
        - Ignora palabras con longitud distinta a 5 caracteres.
        - Gestiona la memoria dinámica para almacenar las palabras.
*/
Palabras leerArchivo(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        perror("Error al abrir archivo");
        return (Palabras){.arr = NULL, .n = 0};
    }

    Palabras palabras = {.arr = malloc(MAX_PALABRAS * sizeof(char*)), .n = 0};
    if (!palabras.arr) {
        perror("Error al asignar memoria");
        fclose(archivo);
        return palabras;
    }

    char buf[16];
    while (fgets(buf, sizeof(buf), archivo)) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') buf[--len] = '\0';

        if (len == 5) {
            palabras.arr[palabras.n] = strdup(buf);
            if (!palabras.arr[palabras.n]) {
                perror("Error al asignar memoria");
                liberarPalabras(palabras);
                fclose(archivo);
                return (Palabras){.arr = NULL, .n = 0};
            }
            palabras.n++;
            if (palabras.n >= MAX_PALABRAS) break;
        }
    }

    fclose(archivo);
    return palabras;
}


/* 
    seEncuentraEnPalabra
    Comprueba si un carácter se encuentra en una palabra.
    - Argumentos:
        - c: Carácter a buscar.
        - str: Cadena donde se realiza la búsqueda.
    - Retorno:
        - true si el carácter está presente, false en caso contrario.
    - Notas:
        - Usa la función estándar strchr para realizar la búsqueda.
*/
bool seEncuentraEnPalabra(char c, const char* str) {
    return strchr(str, c) != NULL;
}


/* 
    ejemploPrinteoResultados
    Imprime un intento de palabra con colores según los resultados del chequeo.
    - Argumentos:
        - intento: Palabra ingresada por el usuario.
        - resultado: Arreglo con los valores de resultado (verde, amarillo, rojo) para cada letra.
    - Notas:
        - Verde indica letra correcta en la posición correcta.
        - Amarillo indica letra correcta en posición incorrecta.
        - Rojo indica letra incorrecta.
*/
void ejemploPrinteoResultados(const char* intento, const Resultado* resultado) {
    for (int i = 0; i < 5; i++) {
        switch (resultado[i]) {
            case ResultadoVerde:
                printf("\033[1;32m%c\033[0m", intento[i]);
                break;
            case ResultadoAmarillo:
                printf("\033[1;33m%c\033[0m", intento[i]);
                break;
            case ResultadoRojo:
                printf("\033[1;31m%c\033[0m", intento[i]);
                break;
            default:
                printf("%c", intento[i]);
                break;
        }
    }
    printf("\n");
}


/* 
    chequeoDePalabra
    Compara un intento con la palabra objetivo y devuelve un arreglo de resultados.
    - Argumentos:
        - intento: Palabra ingresada por el usuario.
        - palabra: Palabra objetivo contra la que se compara.
    - Retorno:
        - Un puntero a un arreglo dinámico de Resultados que indica el estado de cada letra.
    - Notas:
        - La memoria para el resultado debe liberarse después de usarla.
*/
Resultado* chequeoDePalabra(const char* intento, const char* palabra) {
    Resultado* resultado = malloc(5 * sizeof(Resultado));
    if (!resultado) {
        perror("Error al asignar memoria para resultado");
        return NULL;
    }

    for (int i = 0; i < 5; i++) {
        resultado[i] = chequeoDeCaracter(intento[i], i, palabra);
    }
    return resultado;
}


/* 
    chequeoDeCaracter
    Determina el resultado de un carácter comparado con la palabra objetivo.
    - Argumentos:
        - intento: Carácter del intento.
        - index: Índice del carácter en la palabra.
        - palabra: Palabra objetivo contra la que se compara.
    - Retorno:
        - ResultadoVerde si el carácter coincide en posición y letra.
        - ResultadoAmarillo si el carácter existe en otra posición.
        - ResultadoRojo si el carácter no está presente.
*/
Resultado chequeoDeCaracter(char intento, int index, const char* palabra) {
    if (intento == palabra[index]) return ResultadoVerde;
    if (seEncuentraEnPalabra(intento, palabra)) return ResultadoAmarillo;
    return ResultadoRojo;
}


/* 
    liberarPalabras
    Libera la memoria dinámica asignada a una estructura Palabras.
    - Argumentos:
        - palabras: La estructura Palabras a liberar.
    - Notas:
        - Libera cada palabra individualmente y luego el arreglo de punteros.
*/
void liberarPalabras(Palabras palabras) {
    for (int i = 0; i < palabras.n; i++) {
        free(palabras.arr[i]);
    }
    free(palabras.arr);
}

int main() {
    Palabras palabras = leerArchivo("palabras.txt");
    if (!palabras.arr) {
        printf("Error: no se pudieron cargar palabras.\n");
        return 1;
    }

    printf("Se cargaron %d palabras.\n", palabras.n);
    srand((unsigned)time(NULL));
    const char* palabraObjetivo = palabras.arr[rand() % palabras.n];

    printf("Adivina la palabra de 5 letras:\n");

    char intento[16];
    for (int intentos = 0; intentos < 6; intentos++) {
        printf("Intento %d: ", intentos + 1);
        if (!fgets(intento, sizeof(intento), stdin)) {
            printf("Error leyendo entrada.\n");
            break;
        }
        intento[strcspn(intento, "\n")] = '\0';  // Quita el '\n'

        if (strlen(intento) != 5) {
            printf("La palabra debe tener exactamente 5 letras.\n");
            intentos--;
            continue;
        }

        Resultado* resultados = chequeoDePalabra(intento, palabraObjetivo);
        if (!resultados) break;

        ejemploPrinteoResultados(intento, resultados);

        if (strcmp(intento, palabraObjetivo) == 0) {
            printf("¡Felicidades! Adivinaste la palabra.\n");
            free(resultados);
            break;
        }
        free(resultados);

        if (intentos == 5) {
            printf("Lo siento, no adivinaste. La palabra era: %s\n", palabraObjetivo);
        }
    }

    liberarPalabras(palabras);
    return 0;
}
