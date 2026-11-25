#include "Grafo.h"
#include <climits>

// Constructor por defecto, inicializa el grafo vacio; Recibe: nada; Retorna: nada
Grafo::Grafo() {}

// Elimina todos los vertices y sus listas de adyacencia; Recibe: nada; Retorna: void
void Grafo::limpiar() {
    vertices.limpiar();
}

// Consulta si el grafo no tiene vertices; Recibe: nada; Retorna: bool
bool Grafo::estaVacio() const {
    return vertices.estaVacia();
}

// Retorna la cantidad de vertices del grafo; Recibe: nada; Retorna: int
int Grafo::cantidadVertices() const {
    return vertices.longitud();
}

// Busca un vertice por su identificador; Recibe: const string& id; Retorna: int
// Recorre la lista de vertices y retorna la posicion si lo encuentra; si no existe retorna -1.
int Grafo::buscarVertice(const std::string& id) const {
    for (int i = 0; i < vertices.longitud(); i++) {
        if (vertices.obtener(i).id == id) {
            return i;
        }
    }
    return -1;
}

// Agrega un vertice al grafo si no estaba registrado; Recibe: const string& id; Retorna: int
// Si ya existe retorna su indice actual; si no, lo inserta al final y retorna el nuevo indice.
int Grafo::agregarVertice(const std::string& id) {
    int existente = buscarVertice(id);
    if (existente != -1) {
        return existente;
    }
    vertices.agregarAlFinal(Vertice(id));
    return vertices.longitud() - 1;
}

// Obtiene el identificador de un vertice segun su indice; Recibe: int indice; Retorna: string
// Valida que el indice este dentro del rango; si no, retorna cadena vacia.
std::string Grafo::obtenerIdVertice(int indice) const {
    if (indice < 0 || indice >= vertices.longitud()) {
        return "";
    }
    return vertices.obtener(indice).id;
}

// Agrega una arista dirigida entre dos vertices por sus identificadores;
// Recibe: const string& idOrigen, const string& idDestino, int peso; Retorna: bool
bool Grafo::agregarArista(const std::string& idOrigen, const std::string& idDestino, int peso) {
    int idxOrigen = buscarVertice(idOrigen);
    int idxDestino = buscarVertice(idDestino);
    return agregarAristaPorIndice(idxOrigen, idxDestino, peso);
}

// Agrega una arista dirigida usando indices; Recibe: int idxOrigen, int idxDestino, int peso; Retorna: bool
// Valida que ambos indices existan y que el peso no sea negativo (requisito de Dijkstra).
bool Grafo::agregarAristaPorIndice(int idxOrigen, int idxDestino, int peso) {
    int total = vertices.longitud();
    if (idxOrigen < 0 || idxOrigen >= total || idxDestino < 0 || idxDestino >= total || peso < 0) {
        return false;
    }
    vertices.obtener(idxOrigen).adyacentes.agregarAlFinal(Arista(idxDestino, peso));
    return true;
}

/*
===================================================================================
dijkstraMultiOrigen

Recibe: const ListaLineal<int>& origenes, y por referencia distancias, saltos y predecesores.
Retorna: bool (true si se pudo ejecutar correctamente).

Funcionamiento (Paso a paso):
1. Inicializa distancias y saltos en infinito (INT_MAX), predecesores en -1 y
   visitado en false para los V vertices del grafo.
2. Marca cada vertice origen valido (territorios del jugador) con distancia 0 y saltos 0.
3. En cada iteracion selecciona el vertice no visitado con menor distancia (desempatando
   por menor cantidad de saltos). Si no queda ninguno alcanzable, termina.
4. Marca el vertice como visitado y relaja sus aristas salientes: si pasar por el da un
   menor costo al vecino (o igual costo con menos saltos), actualiza distancia, saltos
   y predecesor.
===================================================================================
*/
// [IA] Implementacion y correccion hecha por Gemini 4 Neon: algoritmo de Dijkstra multi-origen
// sobre ListaLineal<T> con desempate por menor cantidad de saltos y control de desbordamiento
// (INT_MAX - arista.peso) compatible con C++98.
bool Grafo::dijkstraMultiOrigen(const ListaLineal<int>& origenes,
                                ListaLineal<int>& distancias,
                                ListaLineal<int>& saltos,
                                ListaLineal<int>& predecesores) const {
    int total = vertices.longitud();
    distancias.limpiar();
    saltos.limpiar();
    predecesores.limpiar();

    if (total == 0 || origenes.estaVacia()) {
        return false;
    }

    ListaLineal<bool> visitado;
    for (int i = 0; i < total; i++) {
        distancias.agregarAlFinal(INT_MAX);
        saltos.agregarAlFinal(INT_MAX);
        predecesores.agregarAlFinal(-1);
        visitado.agregarAlFinal(false);
    }

    // Se inicializan todos los vertices origen con distancia 0 y 0 saltos
    bool hayOrigenValido = false;
    for (int i = 0; i < origenes.longitud(); i++) {
        int idx = origenes.obtener(i);
        if (idx >= 0 && idx < total) {
            distancias.obtener(idx) = 0;
            saltos.obtener(idx) = 0;
            hayOrigenValido = true;
        }
    }
    if (!hayOrigenValido) {
        return false;
    }

    // Ciclo principal de Dijkstra
    for (int paso = 0; paso < total; paso++) {
        int actual = -1;
        for (int v = 0; v < total; v++) {
            if (!visitado.obtener(v) && distancias.obtener(v) != INT_MAX) {
                if (actual == -1 ||
                    distancias.obtener(v) < distancias.obtener(actual) ||
                    (distancias.obtener(v) == distancias.obtener(actual) &&
                     saltos.obtener(v) < saltos.obtener(actual))) {
                    actual = v;
                }
            }
        }

        if (actual == -1) {
            break;
        }

        visitado.obtener(actual) = true;

        // Relajacion de todas las aristas salientes del vertice actual
        const ListaLineal<Arista>& listaAristas = vertices.obtener(actual).adyacentes;
        for (int i = 0; i < listaAristas.longitud(); i++) {
            const Arista& arista = listaAristas.obtener(i);
            int vecino = arista.indiceDestino;
            if (visitado.obtener(vecino)) {
                continue;
            }
            if (distancias.obtener(actual) > INT_MAX - arista.peso) {
                continue;
            }
            int nuevoCosto = distancias.obtener(actual) + arista.peso;
            int nuevosSaltos = saltos.obtener(actual) + 1;

            if (nuevoCosto < distancias.obtener(vecino) ||
                (nuevoCosto == distancias.obtener(vecino) &&
                 nuevosSaltos < saltos.obtener(vecino))) {
                distancias.obtener(vecino) = nuevoCosto;
                saltos.obtener(vecino) = nuevosSaltos;
                predecesores.obtener(vecino) = actual;
            }
        }
    }

    return true;
}

// Reconstruye el camino desde el origen hasta idxDestino;
// Recibe: int idxDestino, const ListaLineal<int>& predecesores, ListaLineal<string>& ruta; Retorna: void
// Retrocede usando predecesores desde el destino hasta llegar a un origen (-1) y luego
// invierte el orden para dejar la ruta en sentido origen -> destino.
void Grafo::reconstruirCamino(int idxDestino,
                              const ListaLineal<int>& predecesores,
                              ListaLineal<std::string>& ruta) const {
    ruta.limpiar();
    int total = vertices.longitud();
    if (idxDestino < 0 || idxDestino >= total || predecesores.longitud() != total) {
        return;
    }

    ListaLineal<std::string> inversa;
    int cursor = idxDestino;
    int pasos = 0;
    while (cursor != -1 && pasos <= total) {
        inversa.agregarAlFinal(vertices.obtener(cursor).id);
        cursor = predecesores.obtener(cursor);
        pasos++;
    }

    for (int i = inversa.longitud() - 1; i >= 0; i--) {
        ruta.agregarAlFinal(inversa.obtener(i));
    }
}
