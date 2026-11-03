#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include "ListaLineal.h"
#include "Carta.h"

// Representa el estado actual del turno el cual tiene 4 opciones.
enum FaseTurno {
    FASE_OBTENER_UNIDADES, // Cuando el jugador recibe sus tropas al inicio de cada turno.
    FASE_ATACAR,           // Cuando el jugador decide si atacar o no un territorio enemigo.
    FASE_FORTIFICAR,       // Cuando el jugador decide si mover tropas entre sus territorios.
    FASE_FINALIZADA        // Cuando finaliza el turno.
};

// =========================================================================
// TAD Jugador
// =========================================================================

class Jugador {
public:
    // =========================================================================
    // Atributos de Jugador
    // =========================================================================
    
    // Nombre del jugador (limitado a 8 caracteres al momento de añadirlo)
    std::string nombreJugador;                       
    
    // Color que se usa al momento de asignar un dueño de un territorio específico
    std::string colorFicha;                        
    
    // Cartas las cuales se le otorgan al jugador cuando elimina a otro 
    // o cuando obtiene un territorio (límite de 1 por turno en este caso).
    ListaLineal<Carta> manoCartas;               
    
    // Define el estado del turno actual
    FaseTurno faseTurnoActual;                     
    
    // Define si ha conquistado al menos un territorio en este turno
    bool logroConquistaTurnoActual;            

    // Define si el jugador ya ejecuto la fase de ataque en este turno
    bool haAtacadoTurnoActual;

    // =========================================================================
    // Constructores
    // =========================================================================
    
    // Constructor por defecto, inicializa todo vacío
    Jugador();
    
    // Constructor con los parámetros, recibe el nombre y color del jugador
    Jugador(const std::string& nom, const std::string& col);

    // =========================================================================
    // Métodos Principales
    // =========================================================================
    
    // Reinicia la fase actual del turno al estado inicial (FASE_OBTENER_UNIDADES)
    // y resetea el flag de conquista.
    void reiniciarFaseTurno();

    // Avanza la máquina de estados a la siguiente fase recibida por parámetro.
    void avanzarFase(FaseTurno siguienteFase);

    // Marca que el jugador ya paso por la fase de ataque en este turno (habilita fortificar).
    // [IA] Generada con Claude (Prompt 12, Apendice A).
    void registrarAtaque();

    // Agrega una nueva carta a la mano del jugador.
    void agregarCarta(const Carta& nuevaCarta);

    // Busca en la mano un grupo valido de 3 cartas para canjear (3 iguales, una de cada
    // tipo, o un comodin + 2 cualquiera). Si lo encuentra deja sus posiciones en
    // i1, i2, i3 y retorna true; si no, retorna false.
    bool buscarGrupoCanjeable(int& i1, int& i2, int& i3) const;

    // Elimina de la mano las 3 cartas ubicadas en las posiciones dadas.
    void eliminarCartas(int i1, int i2, int i3);
};

#endif // JUGADOR_H
