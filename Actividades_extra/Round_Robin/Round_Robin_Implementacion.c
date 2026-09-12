#include "Round_Robin_Declaracion.h"
#include "utilidades_colas.h"


struct ProcesoRep {
    int PID;
    int tiempoDeServicio;
    int timepoDeServicioOriginal;
    int tiempoDeLlegada;
};
typedef struct ProcesoRep *Proceso;


void mostrar_menu(){
    limpiarConsola_COLAS();
    printf("\n------ Round Robin: Simulacion de procesamiento de procesos ------\n");
    printf("Seleccione la opcion [1] para cargar la cola con procesos.\n");
    printf("Seleccione la opcion [2] para procesar la cola de procesos.\n");
    printf("Seleccione la opcion [0] para salir del programa.");
}

void mostrarResultadoDePromediosDeEsperaYRetorno(float promedioRetornos, float promedioEspera){
    printf("\n------ Round Robin: promedios de esperas y de retornos ------\n");
    printf("Tiempo de retorno promedio: %.2f\n", promedioRetornos);
    printf("Tiempo de esperas promedio: %.2f\n", promedioEspera);
}

void cargarProcesosEnCola(Cola colaDeProcesosVacia){
    printf("\n------ Round Robin: Simulacion de carga de procesos en una cola ------\n");
    int cantidadDeProcesos = leer_entero("Cuantos procesos quiere introducir en la cola de procesos? (ADVERTENCIA: SOLO PUEDEN SER COMO MAXIMO 10 PROCESOS Y COMO MINIMO 1) :",1,10);
    int i = 1;
    
    while (i <= cantidadDeProcesos) {
        Proceso p = (Proceso) malloc(sizeof(struct ProcesoRep));
        p->PID = i;
        printf("Seleccione el 'Tiempo de servicio' del proceso '%d': ", p->PID);
        p->tiempoDeServicio = leer_entero("",1,20);
        p->timepoDeServicioOriginal = p->tiempoDeServicio;
        p->tiempoDeLlegada = 0;

        TipoElemento tipo = te_crear_con_valor(i, p);
        c_encolar(colaDeProcesosVacia,tipo);
        i++;
    }
    c_mostrar(colaDeProcesosVacia);
}

int totalTiempoDeServicio(Cola colaDeProcesos) {
    Cola copiaDeColaDeProcesos = c_copiar(colaDeProcesos);
    int sumatoria = 0;
    while (!c_es_vacia(copiaDeColaDeProcesos))
    {
        TipoElemento tipo = c_desencolar(copiaDeColaDeProcesos);
        Proceso proceso = (Proceso) tipo->valor;
        sumatoria += proceso->tiempoDeServicio;
    }
    
    return sumatoria;
}

void promediosDeEsperaYRetorno(Lista lEsperaYRetorno){
    int longitud = l_longitud(lEsperaYRetorno);
    int sumaRetornos = 0, sumaEspera = 0;
    float promedioRetornos = 0.0, promedioEspera = 0.0;
    Iterador listaEyR = iterador(lEsperaYRetorno);
    while (hay_siguiente(listaEyR))
    {
        TipoElemento tipo = siguiente(listaEyR);
        Proceso p = (Proceso) tipo->valor;
        int retorno = tipo->clave - p->tiempoDeLlegada;
        sumaRetornos += retorno;
        sumaEspera += retorno - p->timepoDeServicioOriginal;
    }
    promedioRetornos = (float)sumaRetornos / (float)longitud;
    promedioEspera = (float)sumaEspera / (float)longitud;

    mostrarResultadoDePromediosDeEsperaYRetorno(promedioRetornos,promedioEspera);
}

void planificadorRoundRobin(Cola colaDeProcesos, int totalDeTiempoDeServicio){
    printf("\n------ Round Robin: Simulacion de procesamiento de procesos ------\n");
    int quantum = 4;
    //int tiempoDeIntercambio = 0;
    int reloj = 0;
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
                    printf("\nAl proceso '%d' se le descontaron '%d' de Tiempo de servicio y le quedan '%d'.\n", pActual->PID,quantum,pActual->tiempoDeServicio);
                    c_encolar(colaDeProcesos, tipo);
                } else {

                    if (procesoAnterior != NULL && procesoAnterior->PID != pActual->PID) {
                        printf("\nScheduler de corto plazo inserto un nuevo proceso '%d'\n", pActual->PID);
                    }

                    reloj += pActual->tiempoDeServicio;
                    totalDeTiempoDeServicio -= pActual->tiempoDeServicio;

                    printf("\nAl proceso '%d' se le descontaron '%d' de Tiempo de servicio y le quedan '0'.\n", pActual->PID,pActual->tiempoDeServicio);
                    
                    pActual->tiempoDeServicio = 0;
                    tipo->clave = reloj;
                    l_agregar(listaDeEsperaYRetornos,tipo);
                }
                
                printf("\nProceso '%d' y quantum '%d' y reloj '%d'\n", pActual->PID, quantum, reloj);
            }
            procesoAnterior = pActual;
        }
        rondas += 1;
    }
    
    promediosDeEsperaYRetorno(listaDeEsperaYRetornos);
}
