#ifndef LISTALINEAL_HXX
#define LISTALINEAL_HXX

#include "ListaLineal.h"
#include <cstddef>
#include <stdexcept>

// Es el constructor por defecto; Recibe: nada; Retorna: nada
template <typename T>
ListaLineal<T>::ListaLineal()
    : nodoInicial(NULL), nodoFinal(NULL), cantidadElementos(0) {}

// Es el destructor por defecto; Libera la memoria utilizada por la lista
// [IA] Constructor de copia y operator= generados con Claude (Prompt 12, Apendice A: "corrige
// la copia de ListaLineal para que no compartan nodos").
// Constructor de copia; Recibe: const ListaLineal<T>& otra; Retorna: (constructor)
// Recorre la otra lista y agrega al final una copia de cada valor, en el mismo orden.
template <typename T>
ListaLineal<T>::ListaLineal(const ListaLineal<T> &otra)
    : nodoInicial(NULL), nodoFinal(NULL), cantidadElementos(0) {
  for (Nodo<T> *actual = otra.nodoInicial; actual != NULL;
       actual = actual->siguienteNodo) {
    agregarAlFinal(actual->valorAlmacenado);
  }
}

// Operador de asignacion; Recibe: const ListaLineal<T>& otra; Retorna: ListaLineal<T>&
// Si no es la misma lista, libera los nodos propios y copia los de la otra (copia profunda).
template <typename T>
ListaLineal<T> &ListaLineal<T>::operator=(const ListaLineal<T> &otra) {
  if (this != &otra) {
    limpiar();
    for (Nodo<T> *actual = otra.nodoInicial; actual != NULL;
         actual = actual->siguienteNodo) {
      agregarAlFinal(actual->valorAlmacenado);
    }
  }
  return *this;
}

template <typename T> ListaLineal<T>::~ListaLineal() { limpiar(); }

// Consulta si la lista no contiene elementos; Recibe: nada; Retorna: bool
template <typename T> bool ListaLineal<T>::estaVacia() const {
  return cantidadElementos == 0;
}

// Retorna la cantidad de elementos en la lista; Recibe: nada; Retorna: int
template <typename T> int ListaLineal<T>::longitud() const {
  return cantidadElementos;
}

// Inserta un elemento al final de la lista; Recibe: const T& valor; Retorna:
// void Lo hace al crear un nuevo nodo con el valor recibido, luego revisa si la
// lista esta vacia, si esta vacia el nodo inicial igual al final, si no esta
// vacia le asigna al final de la lista la direccion de memoria del nuevo nodo y
// pone a este mismo como el nodoFinal.
template <typename T> void ListaLineal<T>::agregarAlFinal(const T &valor) {
  Nodo<T> *nuevoNodo = new Nodo<T>(valor);
  if (estaVacia()) {
    nodoInicial = nuevoNodo;
    nodoFinal = nuevoNodo;
  } else {
    nodoFinal->siguienteNodo = nuevoNodo;
    nodoFinal = nuevoNodo;
  }
  cantidadElementos++;
}

// Inserta un elemento manteniendo la lista ordenada; Recibe: const T& valor, bool (*menorQue)(const T&, const T&); Retorna: void
// Crea el nuevo nodo. Si la lista esta vacia el nodo queda como inicial y final. Si el valor
// es menor que el primer elemento, se inserta de primero. En otro caso recorre la lista con un
// apuntador al nodo anterior hasta encontrar el primer elemento que sea mayor que el valor
// (o el final de la lista) e inserta ahi el nodo, asi los elementos iguales conservan su orden.
// Si el nodo queda de ultimo se actualiza el nodoFinal.
template <typename T>
void ListaLineal<T>::insertarOrdenado(const T &valor,
                                      bool (*menorQue)(const T &, const T &)) {
  Nodo<T> *nuevoNodo = new Nodo<T>(valor);
  if (estaVacia()) {
    nodoInicial = nuevoNodo;
    nodoFinal = nuevoNodo;
  } else if (menorQue(valor, nodoInicial->valorAlmacenado)) {
    nuevoNodo->siguienteNodo = nodoInicial;
    nodoInicial = nuevoNodo;
  } else {
    Nodo<T> *anterior = nodoInicial;
    while (anterior->siguienteNodo != NULL &&
           !menorQue(valor, anterior->siguienteNodo->valorAlmacenado)) {
      anterior = anterior->siguienteNodo;
    }
    nuevoNodo->siguienteNodo = anterior->siguienteNodo;
    anterior->siguienteNodo = nuevoNodo;
    if (nuevoNodo->siguienteNodo == NULL) {
      nodoFinal = nuevoNodo;
    }
  }
  cantidadElementos++;
}

// Retorna una referencia al elemento en la posición 'indice'; Recibe: int
// indice; Retorna: T& Busca esa posicion dentro de la lista, si no existe lanza
// un error de out of range, y si existe retorna una referencia al elemento en
// esa misma posicion. retorna un dato editable.
template <typename T> T &ListaLineal<T>::obtener(int indice) {
  if (indice < 0 || indice >= cantidadElementos) {
    throw std::out_of_range("Indice fuera de rango en ListaLineal");
  }
  Nodo<T> *actual = nodoInicial;
  for (int i = 0; i < indice; i++) {
    actual = actual->siguienteNodo;
  }
  return actual->valorAlmacenado;
}

// Retorna una referencia constante al elemento en la posición 'indice'; Recibe:
// int indice; Retorna: const T& Hace lo mismo que el anterior pero el dato que
// retorna lo retorna de manera no editable.
template <typename T> const T &ListaLineal<T>::obtener(int indice) const {
  if (indice < 0 || indice >= cantidadElementos) {
    throw std::out_of_range("Indice fuera de rango en ListaLineal");
  }
  Nodo<T> *actual = nodoInicial;
  for (int i = 0; i < indice; i++) {
    actual = actual->siguienteNodo;
  }
  return actual->valorAlmacenado;
}

// Elimina el elemento en la posición 'indice' y libera memoria; Recibe: int
// indice; Retorna: bool Esta funcion primero verifica si este valor entra en el
// tamano, si no entra retorna false. Si el indice es 0, el apuntador a eliminar
// apunta al inicial, y el apuntador inicial se cambia a
// nodoInicial->siguienteNodo. Si el indice es mayor a 0, recorre la lista, con
// un apuntador a el dato anterior.
template <typename T> bool ListaLineal<T>::eliminarEn(int indice) {
  if (indice < 0 || indice >= cantidadElementos) {
    return false;
  }
  Nodo<T> *aEliminar = NULL;
  if (indice == 0) {
    aEliminar = nodoInicial;
    nodoInicial = nodoInicial->siguienteNodo;
    if (cantidadElementos == 1) {
      nodoFinal = NULL;
    }
  } else {
    Nodo<T> *anterior = nodoInicial;
    for (int i = 0; i < indice - 1; i++) {
      anterior = anterior->siguienteNodo;
    }
    aEliminar = anterior->siguienteNodo;
    anterior->siguienteNodo = aEliminar->siguienteNodo;
    if (aEliminar == nodoFinal) {
      nodoFinal = anterior;
    }
  }
  delete aEliminar;
  cantidadElementos--;
  return true;
}

// Libera todos los nodos y resetea punteros inicial, final y tamaño a 0;
// Recibe: nada; Retorna: void Recorre la lista, liberando la memoria de cada
// nodo, hasta que la lista este vacia.
template <typename T> void ListaLineal<T>::limpiar() {
  Nodo<T> *actual = nodoInicial;
  while (actual != NULL) {
    Nodo<T> *siguiente = actual->siguienteNodo;
    delete actual;
    actual = siguiente;
  }
  nodoInicial = NULL;
  nodoFinal = NULL;
  cantidadElementos = 0;
}

// Intercambia el contenido con otra lista; Recibe: ListaLineal<T>& otra; Retorna: void
// Intercambia los apuntadores inicial y final y la cantidad de elementos. No crea ni libera nodos.
// [IA] Generada con Claude (Prompt 15, Apendice A).
template <typename T> void ListaLineal<T>::intercambiar(ListaLineal<T> &otra) {
  Nodo<T> *inicio = nodoInicial;
  Nodo<T> *fin = nodoFinal;
  int cantidad = cantidadElementos;
  nodoInicial = otra.nodoInicial;
  nodoFinal = otra.nodoFinal;
  cantidadElementos = otra.cantidadElementos;
  otra.nodoInicial = inicio;
  otra.nodoFinal = fin;
  otra.cantidadElementos = cantidad;
}

#endif // LISTALINEAL_HXX
