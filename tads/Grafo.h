#ifndef GRAFO_H
#define GRAFO_H

#include <string>
#include "ListaLineal.h"

// =========================================================================
// Estructuras auxiliares del TAD Grafo (Arista y Vertice)
// =========================================================================

// Representa una arista dirigida y ponderada hacia otro vertice del grafo.
struct Arista {
    int indiceDestino; // Indice del vertice destino en la lista de vertices
    int peso;          // Costo (unidades de ejercito) de cruzar hacia el destino

    Arista() : indiceDestino(-1), peso(0) {}
    Arista(int dest, int p) : indiceDestino(dest), peso(p) {}

    int obtenerDestino() const { return indiceDestino; }
    int obtenerPeso() const { return peso; }
    void fijarDestino(int dest) { indiceDestino = dest; }
    void fijarPeso(int p) { peso = p; }
};

// Representa un vertice del grafo con su identificador y su lista de adyacencia.
struct Vertice {
    std::string id;                 // Codigo oficial del territorio (ej. "1.1", "5.6")
    ListaLineal<Arista> adyacentes; // Lista lineal de aristas salientes desde este vertice

    Vertice() : id("") {}
    Vertice(const std::string& identificador) : id(identificador) {}

    std::string obtenerId() const { return id; }
    const ListaLineal<Arista>& obtenerAdyacentes() const { return adyacentes; }
    void fijarId(const std::string& identificador) { id = identificador; }
};

// =========================================================================
// TAD Grafo (Estructura No Lineal: grafo dirigido y ponderado)
// =========================================================================

class Grafo {
public:
    // =========================================================================
    // Atributos de Grafo
    // =========================================================================

    // Coleccion lineal de vertices del grafo; cada vertice guarda sus aristas salientes
    ListaLineal<Vertice> vertices;

    // =========================================================================
    // Constructores
    // =========================================================================

    // Constructor por defecto, inicializa el grafo vacio.
    Grafo();

    // =========================================================================
    // Metodos Principales
    // =========================================================================

    // Elimina todos los vertices y aristas del grafo - O(V + E)
    void limpiar();

    // Consulta si el grafo no tiene vertices - O(1)
    bool estaVacio() const;

    // Retorna la cantidad total de vertices en el grafo - O(1)
    int cantidadVertices() const;

    // Busca un vertice por su identificador y retorna su indice (o -1 si no existe) - O(V)
    int buscarVertice(const std::string& id) const;

    // Agrega un nuevo vertice si no existe y retorna su indice - O(V)
    int agregarVertice(const std::string& id);

    // Retorna el identificador del vertice ubicado en el indice dado (o "" si es invalido) - O(V)
    std::string obtenerIdVertice(int indice) const;

    // Agrega una arista dirigida desde idOrigen hasta idDestino con el peso indicado - O(V)
    bool agregarArista(const std::string& idOrigen, const std::string& idDestino, int peso);

    // Agrega una arista dirigida usando los indices directos de los vertices - O(V)
    bool agregarAristaPorIndice(int idxOrigen, int idxDestino, int peso);

    // Ejecuta el algoritmo de Dijkstra desde multiples vertices origen (todos con distancia 0).
    // Deja en 'distancias' el costo minimo a cada vertice, en 'saltos' la cantidad de aristas
    // del camino (para desempatar por rutas mas cortas) y en 'predecesores' el vertice previo.
    // Retorna true si se pudo ejecutar (grafo no vacio y al menos un origen valido).
    bool dijkstraMultiOrigen(const ListaLineal<int>& origenes,
                             ListaLineal<int>& distancias,
                             ListaLineal<int>& saltos,
                             ListaLineal<int>& predecesores) const;

    // Reconstruye en 'ruta' la secuencia de identificadores de vertices desde el origen hasta
    // 'idxDestino' siguiendo la lista de predecesores generada por Dijkstra.
    void reconstruirCamino(int idxDestino,
                           const ListaLineal<int>& predecesores,
                           ListaLineal<std::string>& ruta) const;
};

#endif // GRAFO_H
