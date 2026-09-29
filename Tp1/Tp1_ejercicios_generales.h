#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#ifndef Tp1_ejercicios_generales
#define Tp1_ejercicios_generales

#include "../libs/colas/headers/colas.h"
#include "../libs/listas/headers/listas.h"
#include "../libs/validaciones/headers/validacion.h"

//Ejercicio 18
void mostrar_menu();
void cargarProcesosEnCola(Cola colaDeProcesosVacia);
float totalTiempoDeServicio(Cola Procesos);
void planificadorRoundRobinSinIntercambio(Cola colaDeProcesos, float totalDeTiempoDeServicio);
void planificadorRoundRobinConIntercambio(Cola colaDeProcesos, float totalDeTiempoDeServicio);
void promediosDeEsperaYRetorno(Lista lEsperaYRetorno, int intercambioO_Si_O_No);
void mostrarResultadoDePromediosDeEsperaYRetorno(float promedioRetornos, float promedioEspera, int intercambioO_Si_O_No);

#endif