#include "../headers/validacion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float leer_float(const char* mensaje, float min, float max) {
    char buffer[100];
    char* endptr;
    float numero;
    //while (getchar() != '\n');
    while (1) {
        printf("%s", mensaje);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {// Lee una línea completa de entrada, incluyendo espacios.
            printf("Error al leer entrada.\n");
            continue;
        }
        buffer[strcspn(buffer, "\n")] = 0;  //Elimina el \n al final del input, entonces reemplaza el salto de linea con fin de string "\0"
        numero = strtof(buffer, &endptr);  //Convierte el string a float. endptr apunta a donde terminó la conversión.
        if (endptr != buffer && *endptr == '\0') {// Verifica que se haya convertido algo (endptr != buffer) y que no hayan quedado caracteres basura después del número (*endptr == '\0').
            if (numero >= min && numero <= max) {// Se asegura de que el número esté en el rango deseado.
                return numero;  
            } else {
                printf("Numero fuera de rango [%.2f - %.2f].\n", min, max);
            }
        } else {
            printf("Entrada invalida. Ingrese un numero flotante.\n");
        }
    }
}

