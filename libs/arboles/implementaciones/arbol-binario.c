#include <stdlib.h>
#include <string.h>
#include "../headers/arbol-binario.h"
#include "../headers/nodo.h"
//#include "../../colas/headers/colas.h"

static const int TAMANIO_MAXIMO = 100;


struct ArbolBinarioRep {
    NodoArbol raiz;
    int cantidad_elementos;
};


ArbolBinario a_crear() {
    ArbolBinario nuevo_arbol = (ArbolBinario) malloc(sizeof(struct ArbolBinarioRep));
    nuevo_arbol->raiz = NULL;
    nuevo_arbol->cantidad_elementos = 0;
    return nuevo_arbol;
}

bool a_es_vacio(ArbolBinario a) {
    return a->raiz == NULL;
}

bool a_es_lleno(ArbolBinario a){
    return (a->cantidad_elementos == TAMANIO_MAXIMO);
}

int a_cantidad_elementos(ArbolBinario a) {
    return a->cantidad_elementos;
}

bool a_es_rama_nula(NodoArbol pa) {
    return pa == NULL;
}

NodoArbol a_raiz(ArbolBinario a) {
    return a->raiz;
}

NodoArbol a_establecer_raiz(ArbolBinario a, TipoElemento te) {
    if (a->raiz != NULL) {
        // TODO Acá debería ser un error
        // No se puede l_insertar la raíz de un árbol que ya tiene raíz
        return a->raiz;
    }

    NodoArbol na = n_crear(te);
    a->raiz = na;
    a->cantidad_elementos++;
    return na;
}

NodoArbol a_conectar_hi(ArbolBinario a, NodoArbol pa, TipoElemento te) {
    // TODO Acá deberíamos chequear que la posición árbol efectivamente esté en el árbol
    if (pa == NULL) {
        return NULL;
    }

    // Chequeamos que la posición árbol no tenga ya la rama que están solicitando l_insertar
    if (pa->hi != NULL) {
        // TODO Deberíamos devolver error
        return pa->hi;
    }

    NodoArbol na = n_crear(te);
    a->cantidad_elementos++;

    pa->hi = na;

    return na;
}

NodoArbol a_conectar_hd(ArbolBinario a, NodoArbol pa, TipoElemento te) {
    // TODO Acá deberíamos chequear que la posición árbol efectivamente esté en el árbol
    if (pa == NULL) {
        return NULL;
    }

    // Chequeamos que la posición árbol no tenga ya la rama que están solicitando l_insertar
    if (pa->hd != NULL) {
        // TODO Deberíamos devolver error
        return pa->hd;
    }

    NodoArbol na = n_crear(te);
    a->cantidad_elementos++;

    pa->hd = na;

    return na;
}


/*
bool aux_arbolBinarioSimilar(NodoArbol a, NodoArbol b) {
    if (a_es_rama_nula(a) && a_es_rama_nula(b)) return true;
    if (a_es_rama_nula(a) || a_es_rama_nula(b)) return false;

    return aux_arbolBinarioSimilar(n_hijoizquierdo(a), n_hijoizquierdo(b)) && aux_arbolBinarioSimilar(n_hijoderecho(a), n_hijoderecho(b));
}

bool arbolBinarioSimilar(ArbolBinario a, ArbolBinario b) {
    return aux_arbolBinarioSimilar(a->raiz, b->raiz);
}

bool aux_arbolesBinarios_Equivalentes(NodoArbol N1, NodoArbol N2) {
    if (a_es_rama_nula(N1) && a_es_rama_nula(N2)) return true;
    if (a_es_rama_nula(N1) || a_es_rama_nula(N2)) return false;
    if (N1->datos->clave != N2->datos->clave) return false;

    return aux_arbolesBinarios_Equivalentes(n_hijoizquierdo(N1), n_hijoizquierdo(N2)) &&
           aux_arbolesBinarios_Equivalentes(n_hijoderecho(N1), n_hijoderecho(N2));
}

bool arbolesBinarios_Equivalentes(ArbolBinario A, ArbolBinario B) {
    return aux_arbolesBinarios_Equivalentes(a_raiz(A), a_raiz(B));
}

bool aux_arbolesBinarios_Espejos(NodoArbol N1, NodoArbol N2) {
    if (a_es_rama_nula(N1) && a_es_rama_nula(N2)) return true;
    if (a_es_rama_nula(N1) || a_es_rama_nula(N2)) return false;
    if (N1->datos->clave != N2->datos->clave) return false;

    return aux_arbolesBinarios_Espejos(n_hijoizquierdo(N1), n_hijoderecho(N2)) &&
           aux_arbolesBinarios_Espejos(n_hijoderecho(N1), n_hijoizquierdo(N2));
}

bool arbolesBinarios_Espejos(ArbolBinario A, ArbolBinario B) {
    return aux_arbolesBinarios_Espejos(a_raiz(A), a_raiz(B));
}
*/

void aux_arbolesBinarios_Similares(NodoArbol a, NodoArbol b, bool *result){
    if (!*result ||(a_es_rama_nula(a)&&a_es_rama_nula(b)))
    {
        return;
    }

    if (a_es_rama_nula(a)!=a_es_rama_nula(b))
    {
        *result = false;
        return;
    }
    
    aux_arbolesBinarios_Similares(n_hijoizquierdo(a),n_hijoizquierdo(b),result);
    aux_arbolesBinarios_Similares(n_hijoderecho(a),n_hijoderecho(b),result);
}

bool arbolesBinarios_Similares(ArbolBinario a, ArbolBinario b){
    if (a_es_vacio(a) && a_es_vacio(b)){
        return true; 
    }
    bool resultado = true;
    aux_arbolesBinarios_Similares(a->raiz,b->raiz,&resultado);
    
    return resultado;
}

void aux_arbolesBinarios_Equivalentes(NodoArbol N1,NodoArbol N2, bool *result){
    if (!*result || (a_es_rama_nula(N1) && a_es_rama_nula(N2))) return;
    
    if (a_es_rama_nula(N1) != a_es_rama_nula(N2)) {
        *result = false;
        return;
    }

    if (N1->datos->clave != N2->datos->clave) {
        *result = false;
        return;
    }
    
    aux_arbolesBinarios_Equivalentes(n_hijoizquierdo(N1),n_hijoizquierdo(N2),result);
    aux_arbolesBinarios_Equivalentes(n_hijoderecho(N1),n_hijoderecho(N2),result);
}

bool arbolesBinarios_Equivalentes(ArbolBinario A, ArbolBinario B){
    if (a_es_vacio(A) && a_es_vacio(B)){
        return true; 
    }
    
    bool resultado =  true;
    aux_arbolesBinarios_Equivalentes(a_raiz(A), a_raiz(B), &resultado);
    
    return resultado;
}

void aux_arbolesBinarios_Espejos(NodoArbol N1,NodoArbol N2, bool *result){
    if (!*result || (a_es_rama_nula(N1) && a_es_rama_nula(N2))) return;
    
    if (a_es_rama_nula(N1) != a_es_rama_nula(N2)) {
        *result = false;
        return;
    }

    if (N1->datos->clave != N2->datos->clave) {
        *result = false;
        return;
    }
    
    aux_arbolesBinarios_Espejos(n_hijoizquierdo(N1),n_hijoderecho(N2),result);
    aux_arbolesBinarios_Espejos(n_hijoderecho(N1),n_hijoizquierdo(N2),result);
}

bool arbolesBinarios_Espejos(ArbolBinario A, ArbolBinario B){
    if (a_es_vacio(A) && a_es_vacio(B)){
        return true; 
    }
    
    bool resultado =  true;
    aux_arbolesBinarios_Espejos(a_raiz(A), a_raiz(B), &resultado);
    
    return resultado;
}
/*
struct NodoArbolRep {
    TipoElemento datos;
    int hi; //Subindice para hi
    int hd; //Subindice para hd
};
typedef struct NodoArbolRep NodoArbol;

struct ArbolBinarioRep {
    NodoArbol * valores; //Al momenento de crear el arbol le asignare el esapcio que ocupara cada elementos con un callo(tamanio max, sizeof(nodos)) y creare el arbol en base al malloc y la estuctura de 'arbolbianriorep'
    int raiz;
    int libres;
    int cantidad_elementos;
}; typedef struct ArbolBinarioRep *Arbol;
*/