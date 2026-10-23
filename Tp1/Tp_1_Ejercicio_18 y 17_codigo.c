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
    printf("\n------ Suite de Planificadores de CPU ------\n");
    printf("Seleccione la opcion [1] para cargar la cola con procesos.\n");
    printf("Seleccione la opcion [2] para procesar la cola con Round Robin SIN intercambio.\n");
    printf("Seleccione la opcion [3] para procesar la cola con Round Robin CON intercambio.\n");
    printf("Seleccione la opcion [4] para procesar la cola con SJF (Sin desalojo) SIN intercambio.\n");
    printf("Seleccione la opcion [5] para procesar la cola con SJF (Sin desalojo) CON intercambio.\n");
    printf("Seleccione la opcion [6] para procesar la cola con FCFS SIN intercambio.\n");
    printf("Seleccione la opcion [7] para procesar la cola con FCFS CON intercambio.\n");
    printf("Seleccione la opcion [0] para salir del programa.\n");
}

void mostrarResultadoDePromediosDeEsperaYRetorno(float promedioRetornos, float promedioEspera, int tipoPlanificador){
    if (tipoPlanificador == 1)
    {
        printf("\n------ Round Robin con intercambio: promedios de esperas y de retornos ------\n");
        printf("Tiempo de retorno promedio: %.2f\n", promedioRetornos);
        printf("Tiempo de esperas promedio: %.2f\n", promedioEspera);
    } else if (tipoPlanificador == 2)
    {
        printf("\n------ Round Robin sin intercambio: promedios de esperas y de retornos ------\n");
        printf("Tiempo de retorno promedio: %.2f\n", promedioRetornos);
        printf("Tiempo de esperas promedio: %.2f\n", promedioEspera);
    } else if (tipoPlanificador == 3)
    {
        printf("\n------ SJF SIN DESALOJO y con intercambio: promedios de esperas y de retornos ------\n");
        printf("Tiempo de retorno promedio: %.2f\n", promedioRetornos);
        printf("Tiempo de esperas promedio: %.2f\n", promedioEspera);
    } else if (tipoPlanificador == 4)
    {
        printf("\n------ SJF SIN DESALOJO y sin intercambio: promedios de esperas y de retornos ------\n");
        printf("Tiempo de retorno promedio: %.2f\n", promedioRetornos);
        printf("Tiempo de esperas promedio: %.2f\n", promedioEspera);
    } else if (tipoPlanificador == 5)
    {
        printf("\n------ FCFS sin intercambio: promedios de esperas y de retornos ------\n");
        printf("Tiempo de retorno promedio: %.2f\n", promedioRetornos);
        printf("Tiempo de esperas promedio: %.2f\n", promedioEspera);
    } else if (tipoPlanificador == 6)
    {
        printf("\n------ FCFS con intercambio: promedios de esperas y de retornos ------\n");
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

void promediosDeEsperaYRetorno(Lista lEsperaYRetorno, int tipoPlanificador){
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

    mostrarResultadoDePromediosDeEsperaYRetorno(promedioRetornos,promedioEspera, tipoPlanificador);
}

void OrdenarColaDeMenorAMayor (Cola colaDeProcesos) {
    int cantidad = c_tamanio(colaDeProcesos);
    bool bandera = true;

    if (cantidad <= 1){
        return;
    }

    TipoElemento *array = (TipoElemento *)malloc(cantidad * sizeof(TipoElemento));

    for (int i = 0; i < cantidad; i++) {
        array[i] = c_desencolar(colaDeProcesos);
    }

    while (bandera == true)
    {
        bandera = false;
        for (int i = 0; i < cantidad-1; i++) {
            Proceso p1 = (Proceso) array[i]->valor;
            Proceso p2 = (Proceso) array[i+1]->valor;

            if (p1->tiempoDeServicio > p2->tiempoDeServicio) {
                TipoElemento temp = array[i];
                array[i] = array[i+1];
                array[i+1] = temp;
                bandera = true;
            }
        }
        
    }

    for (int i = 0; i < cantidad; i++) {
        c_encolar(colaDeProcesos, array[i]);
    }

    free(array);
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


void planificadorSJFsinDesalojoYsinIntercambio(Cola colaDeProcesos, float totalDeTiempoDeServicio){
    printf("\n------ SJF sin desalojo: Simulacion de procesamiento de procesos ------\n");
    float reloj = 0.0;
    Lista listaDeEsperaYRetornos = l_crear();

    OrdenarColaDeMenorAMayor(colaDeProcesos);

    while (totalDeTiempoDeServicio > 0) {
        int cantidadTotalDeProcesos = c_tamanio(colaDeProcesos);
        for(int i = 1; i <= cantidadTotalDeProcesos; i++){
            TipoElemento tipo = c_desencolar(colaDeProcesos);
            Proceso pActual = (Proceso) tipo->valor;
            if (pActual != NULL && pActual->tiempoDeServicio > 0) {
                
                printf("\nScheduler de corto plazo coloco el proceso '%d'\n", pActual->PID);
                
                reloj += pActual->tiempoDeServicio;
                totalDeTiempoDeServicio -= pActual->tiempoDeServicio;
                printf("\nAl proceso '%d' se le descontaron '%.2f' de Tiempo de servicio y le quedan '0'.\n", pActual->PID,pActual->tiempoDeServicio);

                printf("\nScheduler de corto plazo quito el proceso '%d'\n", pActual->PID);   
                
                pActual->tiempoDeServicio = 0.0;
                pActual->tiempoDeFinalizacionDeServicio = reloj;
                l_agregar(listaDeEsperaYRetornos,tipo);

                printf("\nProceso '%d' y reloj '%.2f'\n", pActual->PID, reloj);
                
            }
    
        }
    }
    
    promediosDeEsperaYRetorno(listaDeEsperaYRetornos, 4);
}

void planificadorSJFsinDesalojoYconIntercambio(Cola colaDeProcesos, float totalDeTiempoDeServicio){
    printf("\n------ SJF sin desalojo: Simulacion de procesamiento de procesos ------\n");
    float reloj = 0.0;
    float tiempoDeIntercambio = 1.0;
    Lista listaDeEsperaYRetornos = l_crear();

    OrdenarColaDeMenorAMayor(colaDeProcesos);

    while (totalDeTiempoDeServicio > 0) {
        int cantidadTotalDeProcesos = c_tamanio(colaDeProcesos);
        for(int i = 1; i <= cantidadTotalDeProcesos; i++){
            TipoElemento tipo = c_desencolar(colaDeProcesos);
            Proceso pActual = (Proceso) tipo->valor;
            if (pActual != NULL && pActual->tiempoDeServicio > 0) {
                
                printf("\nScheduler de corto plazo coloco el proceso '%d'\n", pActual->PID);
                reloj = reloj + tiempoDeIntercambio/2.0;
                
                reloj += pActual->tiempoDeServicio;
                totalDeTiempoDeServicio -= pActual->tiempoDeServicio;
                printf("\nAl proceso '%d' se le descontaron '%.2f' de Tiempo de servicio y le quedan '0'.\n", pActual->PID,pActual->tiempoDeServicio);

                printf("\nScheduler de corto plazo quito el proceso '%d'\n", pActual->PID);  
                reloj = reloj + tiempoDeIntercambio/2.0; 
                
                pActual->tiempoDeServicio = 0.0;
                pActual->tiempoDeFinalizacionDeServicio = reloj;
                l_agregar(listaDeEsperaYRetornos,tipo);

                printf("\nProceso '%d' y reloj '%.2f'\n", pActual->PID, reloj);
                
            }
    
        }
    }
    
    promediosDeEsperaYRetorno(listaDeEsperaYRetornos, 3);
}


void planificadorFCFSYsinIntercambio(Cola colaDeProcesos, float totalDeTiempoDeServicio){
    printf("\n------ FCFS: Simulacion de procesamiento de procesos ------\n");
    float reloj = 0.0;
    Lista listaDeEsperaYRetornos = l_crear();
    while (totalDeTiempoDeServicio > 0) {
        int cantidadTotalDeProcesos = c_tamanio(colaDeProcesos);
        for(int i = 1; i <= cantidadTotalDeProcesos; i++){
            TipoElemento tipo = c_desencolar(colaDeProcesos);
            Proceso pActual = (Proceso) tipo->valor;
            if (pActual != NULL && pActual->tiempoDeServicio > 0) {
                
                printf("\nScheduler de corto plazo coloco el proceso '%d'\n", pActual->PID);
                
                reloj += pActual->tiempoDeServicio;
                totalDeTiempoDeServicio -= pActual->tiempoDeServicio;
                printf("\nAl proceso '%d' se le descontaron '%.2f' de Tiempo de servicio y le quedan '0'.\n", pActual->PID,pActual->tiempoDeServicio);

                printf("\nScheduler de corto plazo quito el proceso '%d'\n", pActual->PID);   
                
                pActual->tiempoDeServicio = 0.0;
                pActual->tiempoDeFinalizacionDeServicio = reloj;
                l_agregar(listaDeEsperaYRetornos,tipo);

                printf("\nProceso '%d' y reloj '%.2f'\n", pActual->PID, reloj);
                
            }
    
        }
    }
    
    promediosDeEsperaYRetorno(listaDeEsperaYRetornos, 5);
}

void planificadorFCFSYconIntercambio(Cola colaDeProcesos, float totalDeTiempoDeServicio){
    printf("\n------ FCFS: Simulacion de procesamiento de procesos ------\n");
    float reloj = 0.0;
    float tiempoDeIntercambio = 1.0;
    Lista listaDeEsperaYRetornos = l_crear();
    while (totalDeTiempoDeServicio > 0) {
        int cantidadTotalDeProcesos = c_tamanio(colaDeProcesos);
        for(int i = 1; i <= cantidadTotalDeProcesos; i++){
            TipoElemento tipo = c_desencolar(colaDeProcesos);
            Proceso pActual = (Proceso) tipo->valor;
            if (pActual != NULL && pActual->tiempoDeServicio > 0) {
                
                printf("\nScheduler de corto plazo coloco el proceso '%d'\n", pActual->PID);
                reloj = reloj + tiempoDeIntercambio/2.0;
                
                reloj += pActual->tiempoDeServicio;
                totalDeTiempoDeServicio -= pActual->tiempoDeServicio;
                printf("\nAl proceso '%d' se le descontaron '%.2f' de Tiempo de servicio y le quedan '0'.\n", pActual->PID,pActual->tiempoDeServicio);

                printf("\nScheduler de corto plazo quito el proceso '%d'\n", pActual->PID);  
                reloj = reloj + tiempoDeIntercambio/2.0; 
                
                pActual->tiempoDeServicio = 0.0;
                pActual->tiempoDeFinalizacionDeServicio = reloj;
                l_agregar(listaDeEsperaYRetornos,tipo);

                printf("\nProceso '%d' y reloj '%.2f'\n", pActual->PID, reloj);
                
            }
    
        }
    }
    
    promediosDeEsperaYRetorno(listaDeEsperaYRetornos, 6);
}