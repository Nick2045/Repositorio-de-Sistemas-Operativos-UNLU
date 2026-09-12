#ifndef EJERCICIO_H_INCLUDED
#define EJERCICIO_H_INCLUDED
 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "../../libs/colas/headers/colas.h"
#include "../../libs/listas/headers/listas.h"
#include "../../libs/validaciones/headers/validacion.h"

void mostrar_menu();
void cargarProcesosEnCola(Cola colaDeProcesosVacia);
int totalTiempoDeServicio(Cola Procesos);
void planificadorRoundRobin(Cola colaDeProcesos, int totalDeTiempoDeServicio);
void promediosDeEsperaYRetorno(Lista lEsperaYRetorno);

#endif // EJERCICIO_H_INCLUDED