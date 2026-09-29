#include "Tp1_ejercicios_generales.h"
#include "utilidades_Colas.h"


int main()
{
    int eleccion = -1;
    Cola Procesos = c_crear();
    while (eleccion != 0)
    {
        mostrar_menu();
        eleccion = leer_entero("\nOpcion elegida: ", 0, 3);

        switch (eleccion) {
        case 0:
            printf("\nSaliendo del programa...\n");
            break;
        case 1:
            if (c_es_vacia(Procesos)) {
                cargarProcesosEnCola(Procesos);
                printf("\nLa cola 'Procesos' acaba de ser llenada de procesos.\n");
            } else {
                printf("\nLa cola 'Procesos' ya esta llena de procesos. No es posible volver a llenarla.\n");
            } 
            system("pause");
            eleccion = -1;
            break;
        case 2:
            if (!c_es_vacia(Procesos)) {
                float sumatoriaTotalDeTiempoDeServicio = totalTiempoDeServicio(Procesos);
                planificadorRoundRobinSinIntercambio(Procesos, sumatoriaTotalDeTiempoDeServicio);
                printf("\nLa cola 'Procesos' ha terminado de ser procesada.\n");
            } else {
                printf("\nLa cola 'Procesos' esta vacia.\n");
            } 
            system("pause");
            eleccion = -1;
            break;
        case 3:
            if (!c_es_vacia(Procesos)) {
                float sumatoriaTotalDeTiempoDeServicio = totalTiempoDeServicio(Procesos);
                planificadorRoundRobinConIntercambio(Procesos, sumatoriaTotalDeTiempoDeServicio);
                printf("\nLa cola 'Procesos' ha terminado de ser procesada.\n");
            } else {
                printf("\nLa cola 'Procesos' esta vacia.\n");
            } 
            system("pause");
            eleccion = -1;
            break;
        default:
            printf("\nError desconocido. Se recomienda cerrar el programa.\n");
            eleccion = -1;
            system("pause");
            break;
        }
    }

    printf("\n");
    system("pause");
    return 0;
}