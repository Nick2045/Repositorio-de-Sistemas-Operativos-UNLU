#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#ifndef Utilidades_COLAS
#define Utilidades_COLAS

#include "../../libs/colas/headers/colas.h"
#include "../../libs/listas/headers/listas.h"
#include "../../libs/validaciones/headers/validacion.h"


void c_intercambiar(Cola P, Cola Paux);
void limpiarConsola_COLAS();
Cola rellenarCola(int elementos);
Cola c_copiar(Cola cola);
int c_tamanio(Cola cola);

#endif // EJERCICIO_H_INCLUDED