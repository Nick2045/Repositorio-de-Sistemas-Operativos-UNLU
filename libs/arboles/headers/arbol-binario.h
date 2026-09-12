#ifndef ARBOL_BINARIO_H_INCLUDED
#define ARBOL_BINARIO_H_INCLUDED

#include <stdbool.h>
#include "nodo.h"
#include "../../tipoElemento/headers/tipo_elemento.h"

struct ArbolBinarioRep;
typedef struct ArbolBinarioRep *ArbolBinario;

ArbolBinario a_crear();

bool a_es_vacio(ArbolBinario a);

bool a_es_lleno(ArbolBinario a);

int a_cantidad_elementos(ArbolBinario a);

bool a_es_rama_nula(NodoArbol pa);

NodoArbol a_raiz(ArbolBinario a);

NodoArbol a_establecer_raiz(ArbolBinario a, TipoElemento te);

NodoArbol a_conectar_hi(ArbolBinario a, NodoArbol pa, TipoElemento te);

NodoArbol a_conectar_hd(ArbolBinario a, NodoArbol pa, TipoElemento te);

bool arbolesBinarios_Similares(ArbolBinario a, ArbolBinario b);

bool arbolesBinarios_Equivalentes(ArbolBinario A, ArbolBinario B);

bool arbolesBinarios_Espejos(ArbolBinario A, ArbolBinario B);

#endif // ARBOL_BINARIO_H_INCLUDED
