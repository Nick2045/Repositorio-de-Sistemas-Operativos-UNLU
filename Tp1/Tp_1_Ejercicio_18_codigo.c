#include "Tp1_ejercicios_generales.h"
#include "utilidades_colas.h"


struct ProcesoRep {
    int PID;
    float tiempoDeServicio;
    float tiempoDeServicioOriginal;
    float tiempoDeLlegada;
    float tiempoDeFinalizacionDeServicio;
};
typedef struct ProcesoRep *Proceso;


void mostrar_menu(){
    limpiarConsola_COLAS();
    printf("\n------ Round Robin: Simulacion de procesamiento de procesos ------\n");
    printf("Seleccione la opcion [1] para cargar la cola con procesos.\n");
    printf("Seleccione la opcion [2] para procesar la cola de procesos con un planificador round robin sin intercambio.\n");
    printf("Seleccione la opcion [3] para procesar la cola de procesos con un planificador round robin con intercambio.\n");
    printf("Seleccione la opcion [0] para salir del programa.");
}

void mostrarResultadoDePromediosDeEsperaYRetorno(float promedioRetornos, float promedioEspera, int intercambioO_Si_O_No){
    if (intercambioO_Si_O_No == 1)
    {
        printf("\n------ Round Robin con intercambio: promedios de esperas y de retornos ------\n");
        printf("Tiempo de retorno promedio: %.2f\n", promedioRetornos);
        printf("Tiempo de esperas promedio: %.2f\n", promedioEspera);
    } else if (intercambioO_Si_O_No == 2)
    {
        printf("\n------ Round Robin sin intercambio: promedios de esperas y de retornos ------\n");
        printf("Tiempo de retorno promedio: %.2f\n", promedioRetornos);
        printf("Tiempo de esperas promedio: %.2f\n", promedioEspera);
    } else 
    {
        printf("\n !SE ROMPIO TODO¡ \n");
    }
}



void cargarProcesosEnCola(Cola colaDeProcesosVacia){
    printf("\n------ Round Robin: Simulacion de carga de procesos en una cola ------\n");
    int cantidadDeProcesos = leer_entero("Cuantos procesos quiere introducir en la cola de procesos? (ADVERTENCIA: SOLO PUEDEN SER COMO MAXIMO 10 PROCESOS Y COMO MINIMO 1) :",1,10);
    int i = 1;
    
    while (i <= cantidadDeProcesos) {
        Proceso p = (Proceso) malloc(sizeof(struct ProcesoRep));
        p->PID = i;
        printf("Seleccione el 'Tiempo de servicio' del proceso '%d': ", p->PID);
        p->tiempoDeServicio = leer_float("",1.0,20.0);
        p->tiempoDeServicioOriginal = p->tiempoDeServicio;
        p->tiempoDeLlegada = 0.0;
        p->tiempoDeFinalizacionDeServicio = 0.0;

        TipoElemento tipo = te_crear_con_valor(i, p);
        c_encolar(colaDeProcesosVacia,tipo);
        i++;
    }
    c_mostrar(colaDeProcesosVacia);
}

float totalTiempoDeServicio(Cola colaDeProcesos) {
    Cola copiaDeColaDeProcesos = c_copiar(colaDeProcesos);
    float sumatoria = 0;
    while (!c_es_vacia(copiaDeColaDeProcesos))
    {
        TipoElemento tipo = c_desencolar(copiaDeColaDeProcesos);
        Proceso proceso = (Proceso) tipo->valor;
        sumatoria += proceso->tiempoDeServicio;
    }
    
    return sumatoria;
}

void promediosDeEsperaYRetorno(Lista lEsperaYRetorno, int intercambioO_Si_O_No){
    float longitud = (float)l_longitud(lEsperaYRetorno);
    float sumaRetornos = 0, sumaEspera = 0;
    float promedioRetornos = 0.0, promedioEspera = 0.0;
    Iterador listaEyR = iterador(lEsperaYRetorno);
    while (hay_siguiente(listaEyR))
    {
        TipoElemento tipo = siguiente(listaEyR);
        Proceso p = (Proceso) tipo->valor;
        float retorno = p->tiempoDeFinalizacionDeServicio - p->tiempoDeLlegada;
        sumaRetornos += retorno;
        sumaEspera += retorno - p->tiempoDeServicioOriginal;
        free(p);
    }
    promedioRetornos = sumaRetornos / longitud;
    promedioEspera = sumaEspera / longitud;

    mostrarResultadoDePromediosDeEsperaYRetorno(promedioRetornos,promedioEspera, intercambioO_Si_O_No);
}

void planificadorRoundRobinSinIntercambio(Cola colaDeProcesos, float totalDeTiempoDeServicio){
    printf("\n------ Round Robin: Simulacion de procesamiento de procesos ------\n");
    float quantum = 4.0;
    float reloj = 0.0;
    Proceso procesoAnterior = NULL;
    Lista listaDeEsperaYRetornos = l_crear();
    int rondas = 1;
    while (totalDeTiempoDeServicio > 0) {
        printf("\n\n=============== RONDA: '%d' ===============", rondas);
        int cantidadTotalDeProcesos = c_tamanio(colaDeProcesos);
        for(int i = 1; i <= cantidadTotalDeProcesos; i++){
            TipoElemento tipo = c_desencolar(colaDeProcesos);
            Proceso pActual = (Proceso) tipo->valor;
            if (pActual != NULL && pActual->tiempoDeServicio > 0) {
                
                if (procesoAnterior != NULL && procesoAnterior->PID != pActual->PID){
                    printf("\nScheduler de corto plazo quito proceso anterior '%d'\n", procesoAnterior->PID);
                } else if (procesoAnterior == NULL) {
                    printf("\nScheduler de corto plazo inserto el primer proceso '%d' en la CPU\n", pActual->PID);
                }
                
                if (pActual->tiempoDeServicio > quantum) {

                    if (procesoAnterior != NULL && procesoAnterior->PID != pActual->PID) {
                        printf("\nScheduler de corto plazo inserto un nuevo proceso '%d'\n", pActual->PID);
                    }


                    pActual->tiempoDeServicio -= quantum;
                    totalDeTiempoDeServicio -=  quantum;
                    reloj += quantum;
                    printf("\nAl proceso '%d' se le descontaron '%.2f' de Tiempo de servicio y le quedan '%.2f'.\n", pActual->PID,quantum,pActual->tiempoDeServicio);
                    c_encolar(colaDeProcesos, tipo);
                } else {

                    if (procesoAnterior != NULL && procesoAnterior->PID != pActual->PID) {
                        printf("\nScheduler de corto plazo inserto un nuevo proceso '%d'\n", pActual->PID);
                    }

                    reloj += pActual->tiempoDeServicio;
                    totalDeTiempoDeServicio -= pActual->tiempoDeServicio;

                    printf("\nAl proceso '%d' se le descontaron '%.2f' de Tiempo de servicio y le quedan '0'.\n", pActual->PID,pActual->tiempoDeServicio);
                    
                    pActual->tiempoDeServicio = 0.0;
                    pActual->tiempoDeFinalizacionDeServicio = reloj;
                    l_agregar(listaDeEsperaYRetornos,tipo);
                }
                
                printf("\nProceso '%d' y quantum '%.2f' y reloj '%.2f'\n", pActual->PID, quantum, reloj);
            }
            procesoAnterior = pActual;
        }
        rondas += 1;
    }
    
    promediosDeEsperaYRetorno(listaDeEsperaYRetornos, 2);
}

void planificadorRoundRobinConIntercambio(Cola colaDeProcesos, float totalDeTiempoDeServicio){
    printf("\n------ Round Robin: Simulacion de procesamiento de procesos ------\n");
    float quantum = 4.0;
    float tiempoDeIntercambio = quantum/10.0;
    float reloj = 0.0;
    Proceso procesoAnterior = NULL;
    Lista listaDeEsperaYRetornos = l_crear();
    int rondas = 1;
    while (totalDeTiempoDeServicio > 0) {
        printf("\n\n=============== RONDA: '%d' ===============", rondas);
        int cantidadTotalDeProcesos = c_tamanio(colaDeProcesos);
        for(int i = 1; i <= cantidadTotalDeProcesos; i++){
            TipoElemento tipo = c_desencolar(colaDeProcesos);
            Proceso pActual = (Proceso) tipo->valor;
            if (pActual != NULL && pActual->tiempoDeServicio > 0) {
                
                if (procesoAnterior != NULL && procesoAnterior->PID != pActual->PID){
                    printf("\nScheduler de corto plazo quito proceso anterior '%d'\n", procesoAnterior->PID);
                    reloj = reloj + tiempoDeIntercambio/2.0;
                } else if (procesoAnterior == NULL) {
                    printf("\nScheduler de corto plazo inserto el primer proceso '%d' en la CPU\n", pActual->PID);
                    reloj = reloj + tiempoDeIntercambio/2.0;
                }
                
                if (pActual->tiempoDeServicio > quantum) {

                    if (procesoAnterior != NULL && procesoAnterior->PID != pActual->PID) {
                        printf("\nScheduler de corto plazo inserto un nuevo proceso '%d'\n", pActual->PID);
                        reloj = reloj + tiempoDeIntercambio/2.0;
                    }


                    pActual->tiempoDeServicio -= quantum;
                    totalDeTiempoDeServicio -=  quantum;
                    reloj += quantum;
                    printf("\nAl proceso '%d' se le descontaron '%.2f' de Tiempo de servicio y le quedan '%.2f'.\n", pActual->PID,quantum,pActual->tiempoDeServicio);
                    c_encolar(colaDeProcesos, tipo);
                } else {

                    if (procesoAnterior != NULL && procesoAnterior->PID != pActual->PID) {
                        printf("\nScheduler de corto plazo inserto un nuevo proceso '%d'\n", pActual->PID);
                        reloj = reloj + tiempoDeIntercambio/2.0;
                    }

                    reloj += pActual->tiempoDeServicio;
                    totalDeTiempoDeServicio -= pActual->tiempoDeServicio;

                    printf("\nAl proceso '%d' se le descontaron '%.2f' de Tiempo de servicio y le quedan '0'.\n", pActual->PID,pActual->tiempoDeServicio);
                    
                    pActual->tiempoDeServicio = 0.0;
                    pActual->tiempoDeFinalizacionDeServicio = reloj;
                    l_agregar(listaDeEsperaYRetornos,tipo);
                }
                
                printf("\nProceso '%d' y quantum '%.2f' y reloj '%.2f'\n", pActual->PID, quantum, reloj);
            }
            procesoAnterior = pActual;
        }
        rondas += 1;
    }
    
    promediosDeEsperaYRetorno(listaDeEsperaYRetornos, 1);
}
