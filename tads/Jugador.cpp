#include "Jugador.h"

// Constructor por defecto, inicializa los valores en blanco y la fase inicial; Recibe: nada; Retorna: nada
Jugador::Jugador() 
    : nombreJugador(""), colorFicha(""), faseTurnoActual(FASE_OBTENER_UNIDADES), logroConquistaTurnoActual(false), haAtacadoTurnoActual(false) {}

// Constructor parametrizado con nombre y color; Recibe: const string& nom, const string& col; Retorna: nada
Jugador::Jugador(const std::string& nom, const std::string& col)
    : nombreJugador(nom), colorFicha(col), faseTurnoActual(FASE_OBTENER_UNIDADES), logroConquistaTurnoActual(false), haAtacadoTurnoActual(false) {}

// Reinicia la fase actual del turno al estado inicial; Recibe: nada; Retorna: void
// Asigna FASE_OBTENER_UNIDADES y cambia los flags de conquista y de ataque a false.
void Jugador::reiniciarFaseTurno() {
    faseTurnoActual = FASE_OBTENER_UNIDADES;
    logroConquistaTurnoActual = false;
    haAtacadoTurnoActual = false;
}

// Avanza la maquina de estados del turno; Recibe: FaseTurno siguienteFase; Retorna: void
// Asigna a faseTurnoActual el valor de la siguiente fase recibida por parametro.
void Jugador::avanzarFase(FaseTurno siguienteFase) {
    faseTurnoActual = siguienteFase;
}

// Marca que el jugador ya ataco en el turno; Recibe: nada; Retorna: void
// [IA] Generada con Claude (Prompt 12, Apendice A).
void Jugador::registrarAtaque() {
    haAtacadoTurnoActual = true;
}

// Agrega una nueva carta a la mano del jugador; Recibe: const Carta& nuevaCarta; Retorna: void
// Inserta la carta al final de la lista lineal de cartas.
void Jugador::agregarCarta(const Carta& nuevaCarta) {
    manoCartas.agregarAlFinal(nuevaCarta);
}

// Busca un grupo valido de 3 cartas canjeables; Recibe: int& i1, int& i2, int& i3; Retorna: bool
// Prioridad: 3 cartas del mismo tipo, luego una de cada tipo y por ultimo un comodin con 2 cartas cualquiera.
// Si encuentra el grupo deja sus posiciones en i1, i2, i3 y retorna true; si no, retorna false.
bool Jugador::buscarGrupoCanjeable(int& i1, int& i2, int& i3) const {
    int total = manoCartas.longitud();
    if (total < 3) {
        return false;
    }

    // Posiciones de las cartas de cada tipo (INFANTERIA, CABALLERIA, ARTILLERIA, COMODIN)
    int posiciones[4][3] = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};
    int conteo[4] = {0, 0, 0, 0};
    for (int i = 0; i < total; i++) {
        int tipo = (int)manoCartas.obtener(i).clasificacion;
        if (conteo[tipo] < 3) {
            posiciones[tipo][conteo[tipo]] = i;
        }
        conteo[tipo]++;
    }

    // 1. Tres cartas del mismo tipo
    for (int tipo = 0; tipo < 3; tipo++) {
        if (conteo[tipo] >= 3) {
            i1 = posiciones[tipo][0];
            i2 = posiciones[tipo][1];
            i3 = posiciones[tipo][2];
            return true;
        }
    }

    // 2. Una carta de cada tipo
    if (conteo[0] >= 1 && conteo[1] >= 1 && conteo[2] >= 1) {
        i1 = posiciones[0][0];
        i2 = posiciones[1][0];
        i3 = posiciones[2][0];
        return true;
    }

    // 3. Un comodin que reemplaza cualquier carta, junto con otras 2 cartas cualquiera
    if (conteo[3] >= 1) {
        int usadas = 0;
        int elegidas[3] = {-1, -1, -1};
        elegidas[usadas++] = posiciones[3][0];
        for (int i = 0; i < total && usadas < 3; i++) {
            if (i != posiciones[3][0]) {
                elegidas[usadas++] = i;
            }
        }
        i1 = elegidas[0];
        i2 = elegidas[1];
        i3 = elegidas[2];
        return true;
    }
    return false;
}

// Elimina 3 cartas de la mano; Recibe: int i1, int i2, int i3 (posiciones distintas); Retorna: void
// Ordena las posiciones de mayor a menor para que cada eliminacion no desplace a las siguientes.
void Jugador::eliminarCartas(int i1, int i2, int i3) {
    int indices[3] = {i1, i2, i3};
    for (int a = 0; a < 2; a++) {
        for (int b = a + 1; b < 3; b++) {
            if (indices[b] > indices[a]) {
                int tmp = indices[a];
                indices[a] = indices[b];
                indices[b] = tmp;
            }
        }
    }
    for (int k = 0; k < 3; k++) {
        manoCartas.eliminarEn(indices[k]);
    }
}
