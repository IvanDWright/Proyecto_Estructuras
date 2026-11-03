#ifndef LISTALINEAL_H
#define LISTALINEAL_H

#include <iostream>

// =========================================================================
// Estructura Nodo genérica
// =========================================================================

template <typename T>
struct Nodo {
    // Guarda el tipo de dato dentro de cada nodo.
    T valorAlmacenado;
    
    // Guarda la posicion de memoria del siguiente nodo
    Nodo<T>* siguienteNodo;

    // El constructor de el nodo se hace con una referencia constante (const T& valor)
    // de ahi se saca el valor y por default el siguiente Nodo es nulo.
    Nodo(const T& valor) : valorAlmacenado(valor), siguienteNodo(NULL) {}
};

// =========================================================================
// TAD ListaLineal<T> (Estructura Lineal Dinámica Genérica)
// =========================================================================

template <typename T>
class ListaLineal {
private:
    Nodo<T>* nodoInicial;
    Nodo<T>* nodoFinal;
    int cantidadElementos;

public:
    // =========================================================================
    // Constructores y Destructores
    // =========================================================================

    // Inicializa la lista vacía
    ListaLineal();

    // [IA] Constructor de copia y operador= generados con Claude (Prompt 12, Apendice A).
    // Copia profunda: crea nodos nuevos con copias de los valores, para que dos listas
    // nunca compartan nodos ni los liberen dos veces.
    ListaLineal(const ListaLineal<T>& otra);
    ListaLineal<T>& operator=(const ListaLineal<T>& otra);

    // Libera toda la memoria dinámica de los nodos
    ~ListaLineal();

    // =========================================================================
    // Métodos Principales
    // =========================================================================

    // Consulta si la lista no contiene elementos - O(1)
    bool estaVacia() const;

    // Retorna la cantidad de elementos en la lista - O(1)
    int longitud() const;

    // Inserta un elemento al final de la lista - O(1)
    void agregarAlFinal(const T& valor);

    // Inserta un elemento manteniendo la lista ordenada de menor a mayor
    // según la función 'menorQue' recibida por parámetro. Si hay elementos "iguales"
    // el nuevo se ubica después de ellos (inserción estable) - O(N).
    void insertarOrdenado(const T& valor, bool (*menorQue)(const T&, const T&));

    // Retorna una referencia al elemento en la posición 'indice' - O(N)
    T& obtener(int indice);
    
    // Retorna una referencia constante al elemento en la posición 'indice' - O(N)
    const T& obtener(int indice) const;

    // Elimina el elemento en la posición 'indice' y libera memoria - O(N)
    bool eliminarEn(int indice);

    // Libera todos los nodos y resetea los punteros inicial, final y cantidad a 0 - O(N)
    void limpiar();

    // Intercambia el contenido con otra lista sin copiar ni mover nodos - O(1).
    // Los nodos no cambian de direccion, asi que los punteros a sus valores siguen siendo validos.
    // [IA] Generada con Claude (Prompt 15, Apendice A: "inicializacion segura").
    void intercambiar(ListaLineal<T>& otra);
};

#include "ListaLineal.hxx"

#endif // LISTALINEAL_H
