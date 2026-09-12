#include "utilidades_colas.h"

void limpiarConsola_COLAS()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

Cola rellenarCola(int elementos)
{
    // llena una col. el parametro elementos representa el largo de la cola
    // si elementos es pasado con el como -1 pregunta el largo al usuario
    if (elementos == -1)
    {
        elementos = leer_entero("Cual es el largo de esta Cola: ", 0, 10);
    }
    if (elementos == 0)
    { // Cola vacia
        return c_crear();
    }
    Cola resultado = c_crear();
    int aleatorio = leer_entero("Queire llenar la cola aleatoriamente? -> No(0) | Si(1): ", 0, 1);
    if (aleatorio == 1)
    {
        for (int j = 0; j < elementos; j++)
        {
            TipoElemento random = te_crear(rand() % 100);
            c_encolar(resultado, random);
        }
    }
    else
    {
        for (int j = 0; j < elementos; j++)
        {
            char mensaje[100];
            sprintf(mensaje, "Ingrese el elemento %i: ", j+1);
            int elem = leer_entero(mensaje, -1000, 1000);
            TipoElemento elemento = te_crear(elem);
            c_encolar(resultado, elemento);
        }
    }
    return resultado;
}

Cola c_copiar(Cola cola){
    Cola resultado = c_crear();
    Cola colaAuxiliar = c_crear();
    TipoElemento elem;
    while (!c_es_vacia(cola))
    {
        elem = c_desencolar(cola);
        c_encolar(resultado, elem);
        c_encolar(colaAuxiliar, elem);
    }
    while (!c_es_vacia(colaAuxiliar)){
        elem = c_desencolar(colaAuxiliar);
        c_encolar(cola, elem);
    }
    return resultado;
}

int c_tamanio(Cola cola){
    // devuelve el tamanio de la cola
    int tamanio = 0;
    Cola aux = c_crear();
    TipoElemento X = te_crear(0);
    while (!(c_es_vacia(cola)))
    {
        X = c_desencolar(cola);
        c_encolar(aux, X);
        tamanio++;
    }
    c_intercambiar(cola, aux);
    return tamanio;
}

void c_intercambiar(Cola c, Cola aux){
    if (c_es_vacia(aux)) return; // No imprimir nada, solo salir
    TipoElemento X;
    while (!c_es_vacia(aux)) {
        X = c_desencolar(aux);
        c_encolar(c,X);
    }
}