#ifndef JUEGORISK_H
#define JUEGORISK_H

#include <string>
#include "ListaLineal.h"
#include "Jugador.h"
#include "TableroRisk.h"
#include "Grafo.h"

// Define los diferentes estados del juego
enum EstadoJuego {
    ESTADO_NO_INICIALIZADO,
    ESTADO_EN_CURSO,
    ESTADO_TERMINADO
};

// Posibles resultados al inicializar el juego desde un archivo
// (corresponden a los mensajes del comando 'inicializar').
enum ResultadoInicializacion {
    INICIALIZACION_CORRECTA,
    INICIALIZACION_ARCHIVO_ERRONEO,   // No se encuentra o no puede leerse
    INICIALIZACION_ARCHIVO_VACIO,     // No contiene información
    INICIALIZACION_SIN_FORMATO,       // No contiene información en el formato esperado
    // [IA] Valor agregado con Claude (Prompt 16, Apendice A: "inicializar debe poder cargar un
    // archivo generado por guardar"). Texto con el formato correcto cuyas unidades ya no son
    // las iniciales (35/30/25/20): se carga como una partida en curso.
    INICIALIZACION_PARTIDA_EN_CURSO
};

// TAD ResultadoConquista: resultado del calculo de rutas de conquista sobre grafos (Componente 3).
struct ResultadoConquista {
    bool valido;                            // true si se encontro una conquista valida
    bool yaControlado;                      // true si el territorio destino ya pertenece al jugador
    int costoUnidades;                      // Total de unidades enemigas a conquistar en el camino
    std::string codigoOrigen;               // Codigo del territorio propio desde donde se inicia el ataque
    std::string codigoDestino;              // Codigo del territorio destino a conquistar
    ListaLineal<std::string> rutaCodigos;   // Secuencia completa de codigos [origen, ..., destino]

    ResultadoConquista()
        : valido(false), yaControlado(false), costoUnidades(0),
          codigoOrigen(""), codigoDestino("") {}

    bool esValido() const { return valido; }
    bool estaYaControlado() const { return yaControlado; }
    int obtenerCostoUnidades() const { return costoUnidades; }
    std::string obtenerCodigoOrigen() const { return codigoOrigen; }
    std::string obtenerCodigoDestino() const { return codigoDestino; }
    const ListaLineal<std::string>& obtenerRutaCodigos() const { return rutaCodigos; }
};

// =========================================================================
// TAD JuegoRisk
// =========================================================================

class JuegoRisk {
public:
    // =========================================================================
    // Atributos de JuegoRisk
    // =========================================================================
    
    // Estado actual del ciclo de vida del juego
    EstadoJuego estadoJuego;                       
    
    // Colección lineal de jugadores participantes
    ListaLineal<Jugador> listaJugadores;           
    
    // Índice en 'listaJugadores' del jugador con el turno activo
    int turnoActualIndex;                  
    
    // Mapa topológico con los 42 territorios
    TableroRisk tableroJuego;                      
    
    // Nombre del jugador victorioso si finalizó la partida
    std::string nombreGanador;                      

    // Cantidad de grupos de cartas canjeados en toda la partida (define el bono del siguiente canje)
    int gruposCanjeados;

    // =========================================================================
    // Constructores
    // =========================================================================
    
    // Inicializa el juego en ESTADO_NO_INICIALIZADO.
    JuegoRisk();

    // =========================================================================
    // Métodos Principales
    // =========================================================================

    // Verifica si el juego está inicializado.
    bool estaInicializado() const;
    
    // Verifica si el juego está terminado.
    bool estaTerminado() const;

    // Obtiene el jugador en turno.
    Jugador* obtenerJugadorEnTurno();
    
    // Busca un jugador por nombre.
    Jugador* buscarJugadorPorNombre(const std::string& nombreBuscado);
    const Jugador* buscarJugadorPorNombre(const std::string& nombreBuscado) const;

    // Pasa al siguiente turno circularmente.
    void pasarAlSiguienteTurno();

    // Verifica si hay un ganador global en la partida.
    bool verificarGanadorGlobal();

    // =========================================================================
    // Reglas del juego (logica de los comandos, sin entrada/salida por consola)
    // =========================================================================

    // Unidades iniciales de cada jugador segun el numero de jugadores (35, 30, 25 o 20).
    // [IA] Generada con Claude (Prompt 13, Apendice A).
    static int unidadesInicialesPorJugador(int numJugadores);

    // Calcula las unidades base del turno: territorios / 3 + bono de continentes.
    int calcularUnidadesBase(const Jugador& jugador) const;

    // Retorna el bono de tropas del siguiente canje segun los grupos ya canjeados
    // (4, 6, 8, 10, 12, 15, 20, ...) e incrementa el contador.
    int obtenerBonoSiguienteCanje();

    // Canjea el grupo de cartas ubicado en las posiciones dadas. Elimina las cartas de
    // la mano, suma +2 tropas en cada territorio propio (distinto) asociado a las cartas del
    // grupo, deja sus codigos en territoriosBono y retorna el bono de tropas del canje.
    // [IA] Firma modificada con Claude (Prompt 13, Apendice A).
    int canjearCartas(Jugador& jugador, int i1, int i2, int i3, ListaLineal<std::string>& territoriosBono);

    // Lanza un dado de 6 caras (1 a 6).
    int lanzarDado() const;

    // Resuelve una ronda de combate. Lanza min(3, tropasAtq) dados del atacante y
    // min(2, tropasDef) del defensor, los ordena de mayor a menor (quedan en dadosAtq y
    // dadosDef) y compara por pares: gana el atacante solo si su dado es estrictamente mayor.
    // Descuenta las bajas de tropasAtq y tropasDef.
    void resolverRondaCombate(int& tropasAtq, int& tropasDef,
                              ListaLineal<int>& dadosAtq, ListaLineal<int>& dadosDef);

    // Registra la conquista de 'destino' por el jugador con 'tropas' tropas, marca que el
    // jugador conquisto en el turno y verifica si gano la partida. Retorna true si ganó.
    bool conquistarTerritorio(Jugador& jugador, Territorio* destino, int tropas);

    // Si el jugador conquisto algun territorio en el turno le entrega 1 carta (maximo una
    // por turno) y deja la bandera de conquista en false. Retorna true si entrego carta.
    bool otorgarCartaPorConquista(Jugador& jugador);

    // Mueve 'cantidad' tropas de 'origen' a 'destino' (fortificacion). Valida que ambos
    // sean del jugador, que sean vecinos y que quede al menos 1 tropa en origen.
    bool moverTropas(const Jugador& jugador, Territorio* origen, Territorio* destino, int cantidad);

    // [IA] Las 5 operaciones siguientes se generaron con Claude (Prompt 12, Apendice A).
    // Ubica 'cantidad' unidades de refuerzo en 'destino' (comando obtener_unidades).
    // Valida que el territorio sea del jugador y que 1 <= cantidad <= disponibles.
    // Retorna true si las ubico.
    bool ubicarUnidades(const Jugador& jugador, Territorio* destino, int cantidad, int disponibles);

    // Aparta 'tropas' tropas del territorio de origen para un ataque. Valida que el origen
    // sea del jugador y que 1 <= tropas <= tropas del origen - 1 (siempre queda 1).
    // Retorna true si las aparto; si no, no modifica nada.
    bool apartarTropasAtacantes(const Jugador& jugador, Territorio* origen, int tropas);

    // Devuelve al origen las tropas atacantes que sobrevivieron a un ataque fallido o detenido.
    void devolverTropasAtacantes(Territorio* origen, int tropas);

    // Deja en el territorio defensor las tropas que le quedaron despues del combate.
    void actualizarDefensor(Territorio* defensor, int tropasRestantes);

    // Termina la fase de ataque del jugador: marca que ya ataco, pasa su fase a Fortificar y,
    // si conquisto algun territorio en el turno, le entrega su carta. Retorna true si entrego carta.
    bool terminarFaseAtaque(Jugador& jugador);

    // =========================================================================
    // Métodos de almacenamiento de partidas (Componente 2)
    // =========================================================================

    // Configura la partida a partir de un texto con el formato del archivo de
    // inicialización. Valida el formato y, si validarSumaInicial es true, que cada jugador
    // tenga la suma inicial de unidades (35/30/25/20). Retorna true si el texto es correcto;
    // si el texto es invalido retorna false y el juego queda exactamente como estaba antes
    // de la llamada.
    // [IA] Parametro validarSumaInicial agregado con Claude (Prompt 13, Apendice A).
    bool cargarDesdeTexto(const std::string& contenido, bool validarSumaInicial = true);

    // Inicializa el juego leyendo un archivo de texto plano o un archivo binario comprimido
    // con Huffman. Si el nombre termina en ".bin" se intenta primero como comprimido y luego
    // como texto; con otra extension, primero como texto y luego como comprimido.
    // Un texto que solo falla por la suma inicial (partida generada por guardar) se carga
    // como partida en curso. Retorna el resultado de la lectura.
    ResultadoInicializacion inicializarDesdeArchivo(const std::string& nombreArchivo);

    // Genera el texto del estado actual de la partida, con el mismo formato del
    // archivo de inicialización (jugadores y los 42 territorios).
    std::string serializarEstado() const;

    // Guarda el estado de la partida en un archivo de texto plano.
    // Retorna true si se pudo guardar.
    bool guardarEnTexto(const std::string& nombreArchivo) const;

    // Guarda el estado de la partida en un archivo binario comprimido con Huffman.
    // Retorna true si se pudo codificar y guardar.
    bool guardarEnComprimido(const std::string& nombreArchivo) const;

    // =========================================================================
    // Metodos de estrategias de juego sobre grafos (Componente 3)
    // =========================================================================

    // Construye el grafo dirigido y ponderado del tablero desde la perspectiva del color dado.
    // Cada arista hacia un territorio enemigo tiene como peso las unidades que lo defienden
    // (0 si el destino es propio), y guarda en 'origenes' los indices de los territorios del jugador.
    void construirGrafoEstrategia(const std::string& colorJugador,
                                  Grafo& grafo,
                                  ListaLineal<int>& origenes) const;

    // Calcula el costo minimo y la secuencia de territorios para que 'nombreJugador' conquiste
    // 'idTerritorioDestino' partiendo desde su territorio controlado mas cercano.
    ResultadoConquista calcularCostoConquista(const std::string& nombreJugador,
                                              const std::string& idTerritorioDestino) const;

    // Calcula cual territorio enemigo implica la menor perdida (conquista) de unidades de ejercito
    // para 'nombreJugador' entre todos los territorios posibles del tablero.
    ResultadoConquista calcularConquistaMasBarata(const std::string& nombreJugador) const;

private:
    // Lee y valida el texto sobre un juego recien creado (la usa cargarDesdeTexto).
    // [IA] Separada de cargarDesdeTexto con Claude (Prompt 15, Apendice A).
    bool cargarEnJuegoVacio(const std::string& contenido, bool validarSumaInicial);
};

#endif // JUEGORISK_H
