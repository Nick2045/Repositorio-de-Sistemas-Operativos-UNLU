#include "../headers/listas.h"
#include <stdlib.h>
#include <stdio.h>

static const int TAMANIO_MAXIMO = 100; // Constante global que define la capacidad fija máxima de elementos 
                                     // que puede almacenar la lista (el tamaño total del arreglo "edificio").
static const int NULO = -1;// Valor centinela que representa una referencia nula o fin de cadena. 
                          // Se usa -1 porque ningún índice de un arreglo en C puede ser negativo.

struct Nodo {
    TipoElemento datos;// El campo que almacena la información real (el TipoElemento con su clave y valor).

    int siguiente;// El "puntero" en forma de índice entero:
                  // - En los nodos OCUPADOS: Apunta al índice del siguiente nodo en la lista de datos (o NULO si es el final).
                  // - En los nodos LIBRES: Apunta al índice del siguiente casillero desocupado disponible (o NULO si no hay más espacio).
};

struct ListaRep {
    struct Nodo *cursor; // Arreglo dinámico (el "edificio") que contiene todos los nodos, 
                        // tanto los libres como los que tienen datos guardados.
                        
    int inicio; // Cabeza de la cadena de DATOS (ocupados):
                // - Si la lista está vacía: Vale NULO (-1) indicando que no hay ningún elemento.
                // - Si tiene elementos: Guarda el índice exacto del primer nodo de la lista.
                             
    int libre;// Cabeza de la cadena de LIBRES (espacios vacíos):
              // - No representa la "cantidad" de libres, sino el *índice* del primer casillero 
              //   disponible para usar. Va cambiando a medida que sacamos y devolvemos nodos.
                             
    int cantidad;// Contador actual de elementos:
                 // - Arranca en 0 (lista vacía) y puede llegar hasta TAMANIO_MAXIMO (lista llena).
};

struct IteradorRep {
    Lista lista;
    int posicionActual;
};


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// Rutinas del TAD de Lista
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

Lista l_crear() {
    int i = 0;
    Lista nueva_lista = (Lista) malloc(sizeof(struct ListaRep));
    nueva_lista->cursor = calloc(TAMANIO_MAXIMO, sizeof(struct Nodo));
    nueva_lista->cantidad = 0;
    nueva_lista->inicio = NULO; //significa que la cadena de los ocupados esta vacia.

    // Encadeno todos los libres
    for (i=0; i<=(TAMANIO_MAXIMO-2); i++) {
        nueva_lista->cursor[i].siguiente = i+1;
    }

    // Instancio inicio, libre y demas
    nueva_lista->libre = 0;
    nueva_lista->cursor[TAMANIO_MAXIMO-1].siguiente = NULO;
    nueva_lista->inicio = NULO;

    // retorno la lista creada
    return nueva_lista;
}


bool l_es_vacia(Lista lista) {
    return lista->cantidad == 0;
}


bool l_es_llena(Lista lista) {
    return (lista->cantidad == TAMANIO_MAXIMO);
}


int l_longitud(Lista lista) {
    return lista->cantidad;
}


bool l_agregar(Lista lista, TipoElemento elemento) {
    int p;
    int q;
    int anteq;

    // controlo lista llena
    if (l_es_llena(lista)) {
        return false;
    }

    // Tomo el primer libre
    p = lista->libre;
    lista->libre = lista->cursor[p].siguiente;

    // Asigno el dato
    lista->cursor[p].datos = elemento;
    lista->cursor[p].siguiente = NULO;

    // Controlo que no sea el primero de la lista
    if (l_es_vacia(lista)) {
        lista->inicio = p;
    } else {
        // lo ubico al final
        q = lista->inicio;
        while (q != NULO) {
            anteq = q;  //guardo el anterior porque no tengo puntero al anterior
            q = lista->cursor[q].siguiente;
        }
        lista->cursor[anteq].siguiente = p;
    }
    lista->cantidad++;
    return true;
}


bool l_borrar(Lista lista, int clave) {
    if (l_es_vacia(lista)) {
        return false;
    }

    bool borre = false;
    int q;
    int p = lista->inicio;

    // borro las claves que coinciden con el inicio
    while ((p != NULO) && (lista->cursor[p].datos->clave == clave)) {
        q = p;
        lista->inicio = lista->cursor[p].siguiente;
        // recupero el nodo en el libre para no perderlo
        lista->cursor[q].siguiente = lista->libre;
        lista->libre = q;
        // Descuento 1 y arranco de nuevo desde el inicio
        lista->cantidad--;
        p = lista->inicio;
        borre = true;
    }

    // Borro las claves en el resto de la lista
    int qant;
    p = lista->inicio;
    while (p != NULO) {
        // pregunto por uno adelantado
        if (lista->cursor[p].datos->clave == clave) {
            q = p;
            lista->cursor[qant].siguiente = lista->cursor[p].siguiente;
            // Preservo en el libre
            lista->cursor[q].siguiente = lista->libre;
            lista->libre = q;
            lista->cantidad--;
            p = qant;  //vuelvo a tomar el qant para revisar que no existan otras claves iguales
            borre = true;
        } else {
            qant = p;  // guardo el anterior
            p = lista->cursor[p].siguiente;
        }
    }
    return borre;
}


TipoElemento l_buscar(Lista lista, int clave) {
    int p = lista->inicio;
    while (p != NULO) {
        if (lista->cursor[p].datos->clave == clave) {
            return lista->cursor[p].datos;
        }
        p = lista->cursor[p].siguiente;
    }
    return NULL;
}


bool l_insertar(Lista lista, TipoElemento elemento, int pos) {
    // Controla si la posicion ordinal es mayor a la cantidad
    // llama automaticamente al agregar
    if (pos > l_longitud(lista)) {
        l_agregar(lista, elemento);
        return false;
    }

    // Sino asigna espacio tomando del libre
    int p = lista->libre;
    lista->libre = lista->cursor[p].siguiente;
    lista->cursor[p].datos = elemento;
    lista->cursor[p].siguiente = NULO;

    // valida si es la primer posicion
    if (pos == 1) {
        lista->cursor[p].siguiente = lista->inicio;
        lista->inicio = p;
    } else {
        int temp2 = lista->inicio;
        for (int i = 0; i < pos - 2; i++) {
            temp2 = lista->cursor[temp2].siguiente;
        }
        lista->cursor[p].siguiente = lista->cursor[temp2].siguiente;
        lista->cursor[temp2].siguiente = p;
    }
    // Cuenta uno mas
    lista->cantidad++;
    return true;
}


bool l_eliminar(Lista lista, int pos) {
    int p;
    bool borre = false;
    int actual = lista->inicio;

    if (1 <= pos && pos <= l_longitud(lista)) {
        if (pos == 1) {
            p = actual;
            lista->inicio = lista->cursor[actual].siguiente;
            lista->cursor[p].siguiente = lista->libre;
            lista->libre = p;
	    borre = true;
        } else {
            for (int i = 0; i < pos - 2; i++) {
                actual = lista->cursor[actual].siguiente;
            }
            // actual apunta al nodo en posición (pos - 1)
            p = lista->cursor[actual].siguiente; // nodo en pos
            lista->cursor[actual].siguiente = lista->cursor[p].siguiente; // nodo en pos + 1
            lista->cursor[p].siguiente = lista->libre;
            lista->libre = p;
            borre = true;
        }
        lista->cantidad--;
    }
    return borre;
}


TipoElemento l_recuperar(Lista lista, int pos) {
    int temp2 = lista->inicio;
    for (int i = 0; i < pos - 1; i++) {
        temp2 = lista->cursor[temp2].siguiente;
    }
    return lista->cursor[temp2].datos;
}


void l_mostrar(Lista lista) {
    int temp2 = lista->inicio;
    printf("Contenido de la lista: ");
    while (temp2 != NULO) {
        printf("%d ", lista->cursor[temp2].datos->clave);
        temp2 = lista->cursor[temp2].siguiente;
    }
    printf("\n");
}


//Hechas por mi:
bool InsertarOrdenadoDI(Lista lista, TipoElemento elemento){
    // controlo lista llena
    if (l_es_llena(lista)) {
        return false;
    }
    int p;
    int q;
    int anteq;

    // Controlo que no sea el primero de la lista
    if (l_es_vacia(lista)) {
        // Tomo el primer libre
        p = lista->libre;
        lista->libre = lista->cursor[p].siguiente;

        // Asigno el dato
        lista->cursor[p].datos = elemento;
        lista->cursor[p].siguiente = NULO;
        lista->inicio = p;
    } else {
        // lo ubico al final
        q = lista->inicio;
        if (lista->cursor[q].datos->clave == elemento->clave)
        {
            printf("\nEl elemento ya estaba presente en la lista\n");
            return false;
        }
        else if (lista->cursor[q].datos->clave > elemento->clave)
        {
            // Tomo el primer libre
            p = lista->libre;
            lista->libre = lista->cursor[p].siguiente;

            // Asigno el dato
            lista->cursor[p].datos = elemento;
            lista->cursor[p].siguiente = q;
            lista->inicio = p;
        } else {
            anteq = lista->inicio;
            while (q != NULO && lista->cursor[q].datos->clave < elemento->clave) 
            {
                anteq = q;  //guardo el anterior porque no tengo puntero al anterior
                q = lista->cursor[q].siguiente;
            }

            if (q == NULO)
            {
                // Tomo el primer libre
                p = lista->libre;
                lista->libre = lista->cursor[p].siguiente;

                // Asigno el dato
                lista->cursor[p].datos = elemento;
                lista->cursor[p].siguiente = NULO;
                lista->cursor[anteq].siguiente = p;
            }
            else if (lista->cursor[q].datos->clave == elemento->clave)
            {
                printf("\nEl elemento ya estaba presente en la lista\n");
                return false;
            }
            else
            {
                // Tomo el primer libre
                p = lista->libre;
                lista->libre = lista->cursor[p].siguiente;

                // Asigno el dato
                lista->cursor[p].datos = elemento;
                lista->cursor[anteq].siguiente = p;
                lista->cursor[p].siguiente = q;
            }
        }
        
    }
    lista->cantidad++;
    return true;
}


//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
// Rutinas del ITERADOR
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------

Iterador iterador(Lista lista) {
    Iterador iter = (Iterador) malloc(sizeof(struct IteradorRep));
    iter->lista = lista;
    iter->posicionActual = lista->inicio;
    return iter;
}


bool hay_siguiente(Iterador iterador) {
    return (iterador->posicionActual != NULO);
}


TipoElemento siguiente(Iterador iterador) {
    if (iterador->posicionActual != NULO){
        TipoElemento actual = iterador->lista->cursor[iterador->posicionActual].datos;
        iterador->posicionActual = iterador->lista->cursor[iterador->posicionActual].siguiente;
        return actual;
    }
    else {
        return NULL;
    }
}

/*
static const int TAMANIO_MAXIMO = 100;
static const int NULO = -1;

struct Nodo {
    TipoElemento datos;
    int siguiente;
};

struct ListaRep {
    struct Nodo cursor[TAMANIO_MAXIMO];
    int inicio;
    int libre;
    int cantidad;
};

struct IteradorRep {
    Lista lista;
    int posicionActual;
};

int EliminarPosFisica(Lista lista, int pos) {
    if (l_es_vacia(lista)) {
        return -1;  // No se puede eliminar de una lista vacía
    }

    int p, posicionOrdinal;
    int actual = lista->inicio;

    // Verifica que la posición solicitada esté dentro del rango válido
    if (1 <= pos && pos <= l_longitud(lista)) {
        if (pos == 1) {
            // Eliminar el primer nodo
            p = actual;
            lista->inicio = lista->cursor[actual].siguiente; // Nuevo inicio
            lista->cursor[p].siguiente = lista->libre;        // Encadenar en lista de libres
            lista->libre = p;
            posicionOrdinal = 1;
        } else {
            // Avanzar hasta el nodo anterior al que se quiere eliminar
            posicionOrdinal = 1;
            for (int i = 0; i < pos - 2; i++) {
                posicionOrdinal++;
                actual = lista->cursor[actual].siguiente;
            }

            // actual ahora está en posición (pos - 1)
            posicionOrdinal++;  // Marca la posición que se va a eliminar

            p = lista->cursor[actual].siguiente; // Nodo a eliminar (posición pos)
            lista->cursor[actual].siguiente = lista->cursor[p].siguiente; // Saltear el nodo eliminado
            lista->cursor[p].siguiente = lista->libre;  // Agregar a la lista de libres
            lista->libre = p;
        }

        lista->cantidad--;  // Decrementar la cantidad de elementos
    } else {
        posicionOrdinal = -1;  // Posición fuera de rango
    }

    return posicionOrdinal;
}


int l_insertar2(Lista lista, TipoElemento elemento, int pos) {
    int posicionLista = -1;
    if (1 <= pos && pos < l_longitud(lista)) {
        int p = lista->libre;
        lista->libre = lista->cursor[p].siguiente;
        lista->cursor[p].datos = elemento;
        lista->cursor[p].siguiente = NULO;
        if (pos == 1) {
            lista->cursor[p].siguiente = lista->inicio;
            lista->inicio = p;
            posicionLista = 1; //posicion ordinal
        } else {
            posicionLista = 1;
            int temp = lista->inicio;
            for (int i = 0; i < pos - 2; i++) {
                temp = lista->cursor[temp].siguiente;
                posicionLista++; //Avanzara hasta quedarse posicion insertar-1
            }
            posicionLista++;//Aumentandole en 1 ya esta en la posicion de insercion 
            // actual apunta al nodo en posición (pos - 1)
            lista->cursor[p].siguiente = lista->cursor[temp].siguiente;
            lista->cursor[temp].siguiente = p;
        }
        lista->cantidad++;
    }else if (pos >= l_longitud(lista))
    {
        l_agregar(lista,elemento);
        posicionLista = lista->cantidad; //posicion ordinal
    }

    return posicionLista;
}
*/