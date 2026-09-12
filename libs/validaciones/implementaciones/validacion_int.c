#include "../headers/validacion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int leer_entero(const char* mensaje, int min, int max) {
    // inprime "mensaje" y valida la entrada de un entero en el rango "min" - "max" inclusive ambos
    char buffer[100];
    char* endptr;
    int numero;
    //while (getchar() != '\n');

    while (1) {
        // impreime el mensaje de la funcino
        printf("%s", mensaje);
        // copia en buffer la entrada del teclado (stdin) con le tamanio "sizepf(buffer)"
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error al leer entrada.\n");
            continue;
        }
        // Eliminar el salto de línea. 
        // strcspn: Cuenta caracteres de (str1 -> buffer) hasta encontrar cualquier carácter de (str2 -> "\n")
        // strcspn devuelve el indice en donde se encuentra "\n"
        // entonces reemplaza el salto ed line con fin de string "\0"
        buffer[strcspn(buffer, "\n")] = 0;

        // Conversión estricta a entero usando strtol (String to Long).
        // Prototipo: long int strtol(const char *nptr, char **endptr, int base)
        // 
        // ¿Cómo funciona?:
        // 1. Analiza el contenido de 'buffer' interpretándolo en base 10.
        // 2. Convierte los caracteres numéricos válidos a un valor entero largo verdadero (que luego casteamos a 'int'). 
        // O sea, si escribi "12345" en stidin, se guarda en el buffer, y ese "12345" en esta funcion ahora sera 12345.
        // 3. Modifica el puntero 'endptr' para que apunte exactamente a la dirección de memoria 
        //    del *primer carácter no convertido* (es decir, el primer carácter inválido o el '\0' final). O sea, si ingrese "12345",
        //    al final siempre estara de ese texto "12345\0", que marca el fin del texto. Lo que se hace aca es guardar la direccion de ese
        //    \0, pero esto solo pasas si ingrese algo valido, si ingresa un "12Hola345", esto va a fallar mas adelante.
        numero = (int)strtol(buffer, &endptr, 10);

        // Validación integral de la entrada:
        // 1. (endptr != buffer): 
        //    'strtol' modifica el puntero 'endptr' para que apunte al lugar donde terminó. 
        //    Si la entrada está vacía ("") o empieza con una letra ("hola"), 'strtol' no puede 
        //    convertir nada y deja 'endptr' apuntando exactamente al inicio del buffer. 
        //    Por lo tanto, exigir (endptr != buffer) garantiza que la función logró procesar 
        //    al menos un carácter numérico al inicio, evitando aceptar entradas vacías o inválidas desde el vamos

        // 2. (*endptr == '\0'): Asegura que la conversión abarcó *toda* la cadena limpiamente, 
        //    garantizando que no quedaron caracteres basura o espacios intermedios no numéricos 
        //    (bloqueando casos como "30 fa", "40xx", etc.).
        if (endptr != buffer && *endptr == '\0') {
            // Valida que el número convertido se encuentre dentro del rango permitido [min, max].
            if (numero >= min && numero <= max) {
                return numero;
            } else {
                printf("Numero fuera de rango [%d - %d].\n", min, max);
            }
        } else {
            printf("Entrada invalida. Ingrese un numero entero.\n");
        }
    }
}
/*
int leer_entero(const char* mensaje, int min, int max) {
    char buffer[100];     // Donde se guarda la entrada del usuario como texto
    char* endptr;         // Se usa para validar si lo ingresado fue realmente un número
    int numero;

    while (1) {
        // Muestra el mensaje que se pasó como parámetro
        printf("%s", mensaje);

        // Lee la línea completa del usuario (hasta Enter)
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error al leer entrada.\n");
            continue;  // Vuelve a pedir la entrada
        }

        // Quita el salto de línea '\n' que pone fgets al final
        buffer[strcspn(buffer, "\n")] = 0;

        // Intenta convertir el texto a un número entero
        numero = (int)strtol(buffer, &endptr, 10);

        // Verifica si:
        // 1. Se pudo convertir al menos un carácter (endptr != buffer)
        // 2. No quedó texto basura después del número (*endptr == '\0')
        if (endptr != buffer && *endptr == '\0') {
            // Si el número está dentro del rango permitido, lo devuelve
            if (numero >= min && numero <= max) {
                return numero;
            } else {
                printf("Número fuera de rango [%d - %d].\n", min, max);
            }
        } else {
            // Si no se pudo convertir correctamente
            printf("Entrada inválida. Ingrese un número entero.\n");
        }
    }
}
*/