# Wordle en C

Este es un programa implementado en C que replica el popular juego **Wordle**, adaptado para el idioma español. El programa permite al usuario intentar adivinar una palabra de 5 letras seleccionada al azar de una lista predefinida.

## ¿Qué es Wordle?

Wordle es un juego de palabras donde el jugador tiene un número limitado de intentos (en este caso, 6) para adivinar una palabra secreta. Cada intento da pistas sobre qué tan cerca está el jugador de adivinar la palabra:

- **Verde:** La letra está en la posición correcta.
- **Amarillo:** La letra está en la palabra, pero en una posición incorrecta.
- **Rojo:** La letra no está en la palabra.

## Ejecución del programa

### Requisitos

- Un compilador de C como `gcc`.
- Un archivo `palabras.txt` que contenga palabras de 5 letras, cada una en una línea.

### Instrucciones

1. **Compila el programa**:
   ```bash
   gcc -o wordle wordle.c
   ```

2. **Ejecuta el programa**:
   ```bash
   ./wordle
   ```

3. **Jugabilidad**:
   - El programa cargará palabras del archivo `palabras.txt`.
   - Elegirá una palabra secreta al azar.
   - Te pedirá ingresar palabras de 5 letras.
   - Después de cada intento, mostrará las pistas (colores) indicando qué tan cerca estás.

### Ejemplo

```plaintext
Se cargaron 1000 palabras.
Adivina la palabra de 5 letras:
Intento 1: banco
b ⬛ a 🟨 n 🟩 c ⬛ o ⬛
Intento 2: canto
c ⬛ a 🟩 n 🟩 t ⬛ o ⬛
...
¡Felicidades! Adivinaste la palabra.
```

## Desarrollo y abstracciones clave

### Estructura del código

El programa está dividido en funciones que encapsulan tareas específicas, siguiendo principios básicos de modularidad y claridad:

1. **Lectura de archivo (leerArchivo)**:
   - Carga palabras desde un archivo y las almacena en una estructura dinámica (`Palabras`).
   - Uso de memoria dinámica con `malloc` para manejar un número arbitrario de palabras.
   - Filtrado de palabras que no tienen exactamente 5 letras.

2. **Chequeo de letras (`chequeoDePalabra` y `chequeoDeCaracter`)**:
   - Determina si las letras del intento coinciden con las de la palabra secreta.
   - `Verde`: Coincide letra y posición.
   - `Amarillo`: Coincide la letra, pero en una posición incorrecta.
   - `Rojo`: La letra no está en la palabra.

3. **Impresión con colores (`ejemploPrinteoResultados`)**:
   - Muestra los resultados con colores en la terminal usando secuencias ANSI.

4. **Limpieza de memoria (`liberarPalabras`)**:
   - Libera la memoria dinámica usada para las palabras y evita fugas de memoria.

### Estructura de datos: `Palabras`
Se utiliza una estructura para manejar la lista de palabras cargadas:

```c
typedef struct {
    char** arr; // Arreglo de punteros a palabras
    int n;      // Número de palabras cargadas
} Palabras;
```

### Definición de colores
Se define un sistema de resultados usando `#define` para asignar valores simbólicos a los colores:

```c
#define ResultadoVerde 1
#define ResultadoAmarillo 2
#define ResultadoRojo 4
```

Esto facilita la interpretación y comparación de resultados en el código.

## Validaciones y robustez

- **Verificación de entradas del usuario**:
  - Las palabras ingresadas deben tener exactamente 5 letras.
  - Uso de `fgets` para prevenir desbordamiento de buffer.

- **Manejo de errores**:
  - En la lectura del archivo y asignación de memoria se utilizan funciones como `perror` para informar errores específicos.

## Consideraciones Técnicas

1. **Uso de Memoria**:
   - Las palabras se almacenan en un arreglo dinámico para manejar una cantidad variable de datos.
   - La memoria es liberada al final del programa para evitar fugas.

2. **Aleatoriedad**:
   - La palabra secreta se selecciona utilizando `rand()` después de inicializar el generador de números aleatorios con `srand(time(NULL))`.

3. **Validación de Entradas**:
   - Verifica que las palabras ingresadas tengan exactamente 5 caracteres.
   - Ignora palabras no válidas del archivo de entrada.

4. **Compatibilidad**:
   - Diseñado para ejecutarse en cualquier sistema que tenga un compilador de C estándar.
   - Usa secuencias ANSI para colores, compatibles con la mayoría de las terminales.

## Preguntas Frecuentes (FAQ)

### 1. ¿Cómo agrego más palabras al juego?
   - Edita el archivo `palabras.txt` y añade palabras de 5 letras, una por línea.

### 2. ¿Qué pasa si el archivo `palabras.txt` está vacío?
   - El programa mostrará un error indicando que no se pudieron cargar palabras.

### 3. ¿Qué terminales son compatibles con los colores?
   - La mayoría de las terminales modernas (como las de Linux, macOS, y la consola de Windows 10/11) son compatibles con las secuencias ANSI usadas para los colores.

### 4. ¿Cómo puedo cambiar el número de intentos?
   - En el código, modifica la condición del bucle `for` en el `main`:
     ```c
     for (int intentos = 0; intentos < 6; intentos++) { ... }
     ```
     Sustituye 6 por el número deseado.

## Créditos
Fuente: https://www.youtube.com/watch?v=NGV8dNPB5J8&t=14306s (Dr Jonas Birch Tutorial)
Inspirado en el popular juego de palabras Wordle.  
Este proyecto fue desarrollado para practicar y enseñar principios clave de programación en C.
