#include "JuegoRisk.h"
#include "ArbolHuffman.h"
#include "ArchivoHuffman.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>

// Constructor por defecto, inicializa en ESTADO_NO_INICIALIZADO; Recibe: nada; Retorna: nada
JuegoRisk::JuegoRisk() 
    : estadoJuego(ESTADO_NO_INICIALIZADO), turnoActualIndex(0), nombreGanador(""), gruposCanjeados(0) {}

// Consulta si el juego esta en curso; Recibe: nada; Retorna: bool
bool JuegoRisk::estaInicializado() const {
    return estadoJuego == ESTADO_EN_CURSO;
}

// Consulta si el juego ya termino; Recibe: nada; Retorna: bool
bool JuegoRisk::estaTerminado() const {
    return estadoJuego == ESTADO_TERMINADO;
}

// Obtiene el jugador con el turno actual; Recibe: nada; Retorna: Jugador*
// Retorna un puntero al jugador actual si el juego esta en curso y la lista no esta vacia; de lo contrario retorna NULL.
Jugador* JuegoRisk::obtenerJugadorEnTurno() {
    if (estadoJuego != ESTADO_EN_CURSO || listaJugadores.estaVacia()) {
        return NULL;
    }
    return &(listaJugadores.obtener(turnoActualIndex));
}

// Busca un jugador por su nombre en la lista; Recibe: const string& nombreBuscado; Retorna: Jugador*
// Recorre la lista de jugadores y retorna un puntero al jugador si lo encuentra; si no existe retorna NULL.
Jugador* JuegoRisk::buscarJugadorPorNombre(const std::string& nombreBuscado) {
    for (int i = 0; i < listaJugadores.longitud(); i++) {
        if (listaJugadores.obtener(i).nombreJugador == nombreBuscado) {
            return &(listaJugadores.obtener(i));
        }
    }
    return NULL;
}

// Pasa al siguiente turno circularmente; Recibe: nada; Retorna: void
// Avanza circularmente el indice de turno usando modulo, saltando a los jugadores que ya no
// controlan ningun territorio (eliminados), y reinicia la fase del turno para el nuevo jugador.
void JuegoRisk::pasarAlSiguienteTurno() {
    if (listaJugadores.estaVacia()) {
        return;
    }

    // Rotación circular en la lista de jugadores (maximo una vuelta completa)
    for (int intentos = 0; intentos < listaJugadores.longitud(); intentos++) {
        turnoActualIndex = (turnoActualIndex + 1) % listaJugadores.longitud();
        if (tableroJuego.cantidadTerritoriosOcupadosPor(listaJugadores.obtener(turnoActualIndex).colorFicha) > 0) {
            break;
        }
    }
    listaJugadores.obtener(turnoActualIndex).reiniciarFaseTurno();
}

// Verifica si hay un ganador global en la partida; Recibe: nada; Retorna: bool
// Si el jugador actual controla los 42 territorios del mapa, cambia el estado a TERMINADO, guarda su nombre y retorna true.
bool JuegoRisk::verificarGanadorGlobal() {
    if (estadoJuego != ESTADO_EN_CURSO) {
        return false;
    }

    Jugador* actual = obtenerJugadorEnTurno();
    if (actual != NULL) {
        if (tableroJuego.cantidadTerritoriosOcupadosPor(actual->colorFicha) == 42) {
            estadoJuego = ESTADO_TERMINADO;
            nombreGanador = actual->nombreJugador;
            return true;
        }
    }
    return false;
}

// =========================================================================
// Reglas del juego (logica de los comandos, sin entrada/salida por consola)
// =========================================================================

// Unidades iniciales de cada jugador; Recibe: int numJugadores; Retorna: int
// Numeral 1.4: 35 para 3 jugadores, 30 para 4, 25 para 5 y 20 para 6 (0 si no es valido).
// [IA] Funcion generada con Claude (Prompt 13, Apendice A).
int JuegoRisk::unidadesInicialesPorJugador(int numJugadores) {
    if (numJugadores < 3 || numJugadores > 6) {
        return 0;
    }
    return 35 - 5 * (numJugadores - 3);
}

// Calcula las unidades base del turno; Recibe: const Jugador& jugador; Retorna: int
// Retorna territorios / 3 (division entera, numeral 1.5 del enunciado) mas el bono de los
// continentes que el jugador domina por completo.
// [IA] Linea modificada con Claude (Prompt 13, Apendice A: "auditoria final ... el enunciado
// oficial tiene prioridad absoluta"): se quito el minimo de 3 unidades, que el enunciado no pide.
int JuegoRisk::calcularUnidadesBase(const Jugador& jugador) const {
    int base = tableroJuego.cantidadTerritoriosOcupadosPor(jugador.colorFicha) / 3;
    return base + tableroJuego.obtenerBonoPorContinentesDominados(jugador.colorFicha);
}

// Obtiene el bono del siguiente canje de cartas; Recibe: nada; Retorna: int
// Incrementa gruposCanjeados y retorna 4, 6, 8, 10, 12, 15 y luego de 5 en 5 (20, 25, ...).
int JuegoRisk::obtenerBonoSiguienteCanje() {
    gruposCanjeados++;
    if (gruposCanjeados <= 5) {
        return 2 + 2 * gruposCanjeados;
    }
    return 15 + (gruposCanjeados - 6) * 5;
}

// Canjea 3 cartas por tropas; Recibe: Jugador& jugador, int i1, int i2, int i3, ListaLineal<string>& territoriosBono; Retorna: int
// Por cada territorio distinto de las 3 cartas que este ocupado por el jugador suma +2 tropas en ese
// territorio y agrega su codigo a territoriosBono (queda vacia si no hubo). Luego elimina las 3 cartas
// de la mano y retorna el bono del canje.
// [IA] Bloque modificado con Claude (Prompt 13, Apendice A): antes solo se daba +2 al primer territorio
// propio; el numeral 1.5 dice "2 unidades extra por cada territorio".
int JuegoRisk::canjearCartas(Jugador& jugador, int i1, int i2, int i3, ListaLineal<std::string>& territoriosBono) {
    territoriosBono.limpiar();
    int indices[3] = {i1, i2, i3};
    for (int k = 0; k < 3; k++) {
        const std::string codigo = jugador.manoCartas.obtener(indices[k]).idAsociado;
        if (codigo == "COMODIN") {
            continue;
        }
        bool repetido = false;
        for (int r = 0; r < territoriosBono.longitud(); r++) {
            if (territoriosBono.obtener(r) == codigo) {
                repetido = true;
            }
        }
        Territorio* ter = tableroJuego.obtenerTerritorioPorId(codigo);
        if (!repetido && ter != NULL && ter->colorPropietario == jugador.colorFicha) {
            ter->modificarEjercitos(2);
            territoriosBono.agregarAlFinal(codigo);
        }
    }
    jugador.eliminarCartas(i1, i2, i3);
    return obtenerBonoSiguienteCanje();
}

// Lanza un dado; Recibe: nada; Retorna: int entre 1 y 6
// Usa rand(), por lo que main debe llamar a srand() una sola vez al inicio para que los
// resultados cambien entre ejecuciones.
int JuegoRisk::lanzarDado() const {
    return rand() % 6 + 1;
}

// Resuelve una ronda de combate; Recibe: int& tropasAtq, int& tropasDef, vector<int>& dadosAtq, vector<int>& dadosDef; Retorna: void
// El atacante lanza min(3, tropasAtq) dados y el defensor min(2, tropasDef). Se ordenan de mayor a
// menor y se comparan por pares; el atacante gana el duelo solo si su dado es mayor que el del defensor.
void JuegoRisk::resolverRondaCombate(int& tropasAtq, int& tropasDef,
                                     std::vector<int>& dadosAtq, std::vector<int>& dadosDef) {
    int numAtq = tropasAtq < 3 ? tropasAtq : 3;
    int numDef = tropasDef < 2 ? tropasDef : 2;
    dadosAtq.assign(numAtq, 0);
    dadosDef.assign(numDef, 0);
    for (int i = 0; i < numAtq; i++) {
        dadosAtq[i] = lanzarDado();
    }
    for (int i = 0; i < numDef; i++) {
        dadosDef[i] = lanzarDado();
    }
    std::sort(dadosAtq.rbegin(), dadosAtq.rend());
    std::sort(dadosDef.rbegin(), dadosDef.rend());

    int duelos = numAtq < numDef ? numAtq : numDef;
    for (int i = 0; i < duelos; i++) {
        if (dadosAtq[i] > dadosDef[i]) {
            tropasDef--;
        } else {
            tropasAtq--;
        }
    }
}

// Registra una conquista; Recibe: Jugador& jugador, Territorio* destino, int tropas; Retorna: bool
// Asigna el territorio al jugador con las tropas sobrevivientes, marca la conquista del turno y
// verifica si el jugador ya domina los 42 territorios (retorna true si gano la partida).
bool JuegoRisk::conquistarTerritorio(Jugador& jugador, Territorio* destino, int tropas) {
    destino->asignarOcupante(jugador.colorFicha, tropas);
    jugador.logroConquistaTurnoActual = true;
    return verificarGanadorGlobal();
}

// Entrega la carta del turno; Recibe: Jugador& jugador; Retorna: bool
// Si el jugador conquisto al menos un territorio en el turno recibe 1 carta de un territorio al azar
// (tipo de tropa segun su posicion, con una pequena probabilidad de comodin) y se reinicia la bandera.
bool JuegoRisk::otorgarCartaPorConquista(Jugador& jugador) {
    if (!jugador.logroConquistaTurnoActual) {
        return false;
    }
    jugador.logroConquistaTurnoActual = false;
    if (rand() % 14 == 0) {
        jugador.agregarCarta(Carta("COMODIN", COMODIN));
        return true;
    }
    int pos = rand() % tableroJuego.listaTerritoriosGlob.longitud();
    const Territorio& ter = tableroJuego.listaTerritoriosGlob.obtener(pos);
    jugador.agregarCarta(Carta(ter.idTerritorio, (TipoTropa)(pos % 3)));
    return true;
}

// Mueve tropas entre territorios propios vecinos; Recibe: const Jugador& jugador, Territorio* origen, Territorio* destino, int cantidad; Retorna: bool
// Retorna false (sin modificar nada) si algun territorio es nulo, si no pertenecen al jugador, si no son
// vecinos, si cantidad <= 0 o si en el origen no quedaria al menos 1 tropa.
bool JuegoRisk::moverTropas(const Jugador& jugador, Territorio* origen, Territorio* destino, int cantidad) {
    if (origen == NULL || destino == NULL) {
        return false;
    }
    if (origen->colorPropietario != jugador.colorFicha || destino->colorPropietario != jugador.colorFicha) {
        return false;
    }
    if (!origen->esVecinoDe(destino->idTerritorio)) {
        return false;
    }
    if (cantidad <= 0 || cantidad > origen->cantidadTropas - 1) {
        return false;
    }
    origen->modificarEjercitos(-cantidad);
    destino->modificarEjercitos(cantidad);
    return true;
}

// =========================================================================
// Métodos de almacenamiento de partidas (Componente 2)
// =========================================================================

// [IA] ubicarUnidades, apartarTropasAtacantes, devolverTropasAtacantes, actualizarDefensor y
// terminarFaseAtaque generadas con Claude (Prompt 12, Apendice A: "mueve a los TADs la logica
// que main modificaba directamente sobre los territorios").
// Ubica unidades de refuerzo; Recibe: jugador, territorio destino, cantidad y unidades disponibles; Retorna: bool
bool JuegoRisk::ubicarUnidades(const Jugador& jugador, Territorio* destino, int cantidad, int disponibles) {
    if (destino == NULL || destino->colorPropietario != jugador.colorFicha) {
        return false;
    }
    if (cantidad <= 0 || cantidad > disponibles) {
        return false;
    }
    destino->modificarEjercitos(cantidad);
    return true;
}

// Aparta las tropas atacantes del origen; Recibe: jugador, territorio de origen y tropas; Retorna: bool
bool JuegoRisk::apartarTropasAtacantes(const Jugador& jugador, Territorio* origen, int tropas) {
    if (origen == NULL || origen->colorPropietario != jugador.colorFicha) {
        return false;
    }
    if (tropas <= 0 || tropas > origen->cantidadTropas - 1) {
        return false;
    }
    origen->modificarEjercitos(-tropas);
    return true;
}

// Devuelve las tropas sobrevivientes al origen; Recibe: territorio de origen y tropas; Retorna: void
void JuegoRisk::devolverTropasAtacantes(Territorio* origen, int tropas) {
    if (origen != NULL && tropas > 0) {
        origen->modificarEjercitos(tropas);
    }
}

// Actualiza las tropas del defensor; Recibe: territorio defensor y tropas que le quedan; Retorna: void
void JuegoRisk::actualizarDefensor(Territorio* defensor, int tropasRestantes) {
    if (defensor != NULL) {
        defensor->asignarOcupante(defensor->colorPropietario, tropasRestantes);
    }
}

// Termina la fase de ataque; Recibe: jugador; Retorna: bool (true si recibio carta)
bool JuegoRisk::terminarFaseAtaque(Jugador& jugador) {
    jugador.registrarAtaque();
    jugador.avanzarFase(FASE_FORTIFICAR);
    return otorgarCartaPorConquista(jugador);
}

/*
===================================================================================
cargarDesdeTexto

Recibe: const std::string& contenido, bool validarSumaInicial. Retorna: bool
(Los pasos 1 a 6 los hace cargarEnJuegoVacio sobre un juego temporal; ver cargarDesdeTexto.)

Funcionamiento (Paso a paso):
1. Lee el numero de jugadores (entre 3 y 6) y limpia el juego actual.
2. Lee los N jugadores (nombre de maximo 8 caracteres y color). No se permiten nombres ni
   colores repetidos.
3. Configura el mapa base y lee los 42 territorios: codigo, color del ocupante y tropas.
   Valida que el territorio exista y no este repetido, que el color sea de un jugador de la
   partida y que tenga al menos 1 unidad de infanteria. No debe haber datos sobrantes al final.
4. Valida que todos los territorios del tablero hayan quedado con unidades asignadas.
5. Si validarSumaInicial es true (archivo de inicio), valida que cada jugador ocupe al menos
   un territorio.
5b. Si validarSumaInicial es true, valida que la suma de unidades de cada jugador sea la
   cantidad inicial del numeral 1.4: 35 (3 jugadores), 30 (4), 25 (5) o 20 (6).
6. Si todo es correcto deja el juego temporal EN_CURSO con el turno en el primer jugador con
   territorios; si no, retorna false y el juego original queda como estaba.
===================================================================================
*/
// Carga segura; Recibe: const string& contenido, bool validarSumaInicial; Retorna: bool
// [IA] Generada con Claude (Prompt 15, Apendice A: "lee y valida todos los datos antes de
// reemplazar el estado existente"). Todo se lee y valida en un juego nuevo y temporal. Solo si el
// texto es correcto se intercambian sus listas con las de este juego (en O(1), sin copiar nodos) y
// se copian el estado y el turno. Si el texto es invalido este juego no se modifica. El estado
// anterior queda en el juego temporal y su destructor lo libera.
bool JuegoRisk::cargarDesdeTexto(const std::string& contenido, bool validarSumaInicial) {
    JuegoRisk nuevo;
    if (!nuevo.cargarEnJuegoVacio(contenido, validarSumaInicial)) {
        return false;
    }
    listaJugadores.intercambiar(nuevo.listaJugadores);
    tableroJuego.intercambiar(nuevo.tableroJuego);
    estadoJuego = nuevo.estadoJuego;
    turnoActualIndex = nuevo.turnoActualIndex;
    nombreGanador = nuevo.nombreGanador;
    gruposCanjeados = nuevo.gruposCanjeados;
    return true;
}

// Lee y valida el texto sobre este juego (se usa sobre un juego recien creado); Retorna: bool
bool JuegoRisk::cargarEnJuegoVacio(const std::string& contenido, bool validarSumaInicial) {
    std::istringstream archivo(contenido);
    bool formatoCorrecto = true;
    int numJugadores = 0;

    // 1. Leer número de jugadores
    if (!(archivo >> numJugadores) || numJugadores < 3 || numJugadores > 6) {
        formatoCorrecto = false;
    }

    listaJugadores.limpiar();
    tableroJuego.vaciarTablero();
    estadoJuego = ESTADO_NO_INICIALIZADO;

    std::vector<std::string> coloresJugadores;

    // 2. Leer jugadores (nombre de máximo 8 caracteres sin espacios y color)
    //    Los nombres y los colores no se pueden repetir entre jugadores.
    if (formatoCorrecto) {
        for (int i = 0; i < numJugadores; ++i) {
            std::string nombre, color;
            if (!(archivo >> nombre >> color) || nombre.size() > 8) {
                formatoCorrecto = false;
                break;
            }
            for (int k = 0; k < listaJugadores.longitud(); ++k) {
                if (listaJugadores.obtener(k).nombreJugador == nombre ||
                    listaJugadores.obtener(k).colorFicha == color) {
                    formatoCorrecto = false;
                }
            }
            if (!formatoCorrecto) {
                break;
            }
            listaJugadores.agregarAlFinal(Jugador(nombre, color));
            coloresJugadores.push_back(color);
        }
    }

    // 3. Inicializar mapa y asignar territorios
    if (formatoCorrecto) {
        tableroJuego.configurarMapaBase();
        for (int i = 0; i < 42; ++i) {
            std::string codTer, colorPropietario;
            int numTropas;
            if (!(archivo >> codTer >> colorPropietario >> numTropas) || numTropas < 1) {
                formatoCorrecto = false;
                break;
            }

            // El color del territorio debe pertenecer a un jugador de la partida
            bool colorValido = false;
            for (size_t j = 0; j < coloresJugadores.size(); ++j) {
                if (coloresJugadores[j] == colorPropietario) {
                    colorValido = true;
                    break;
                }
            }
            if (!colorValido) {
                formatoCorrecto = false;
                break;
            }

            Territorio* ter = tableroJuego.obtenerTerritorioPorId(codTer);
            if (ter == NULL) {
                formatoCorrecto = false; // Territorio no existe
                break;
            }
            if (ter->cantidadTropas > 0) {
                formatoCorrecto = false; // Territorio repetido en el archivo
                break;
            }
            ter->asignarOcupante(colorPropietario, numTropas);
        }

        // No debe haber informacion sobrante despues de los 42 territorios
        std::string sobrante;
        if (formatoCorrecto && (archivo >> sobrante)) {
            formatoCorrecto = false;
        }
    }

    // 4. Todos los territorios del tablero deben tener unidades asignadas
    if (formatoCorrecto) {
        for (int i = 0; i < tableroJuego.listaTerritoriosGlob.longitud(); ++i) {
            if (tableroJuego.listaTerritoriosGlob.obtener(i).cantidadTropas < 1) {
                formatoCorrecto = false;
                break;
            }
        }
    }

    // 5. En un archivo de inicio cada jugador debe ocupar al menos un territorio. En una partida
    //    guardada (.bin) un jugador ya eliminado puede tener 0 territorios.
    // [IA] Condicion validarSumaInicial agregada con Claude (Prompt 13, Apendice A): antes un .bin
    // guardado despues de eliminar a un jugador no se podia volver a cargar.
    if (formatoCorrecto && validarSumaInicial) {
        for (int i = 0; i < listaJugadores.longitud(); ++i) {
            if (tableroJuego.cantidadTerritoriosOcupadosPor(listaJugadores.obtener(i).colorFicha) < 1) {
                formatoCorrecto = false;
                break;
            }
        }
    }

    // 5b. Suma inicial de unidades por jugador (numeral 1.4 y comando inicializar, numeral 2.1)
    // [IA] Bloque generado con Claude (Prompt 13, Apendice A: "audita contra el enunciado ...
    // corrige lo que no cumpla"). Solo se aplica a archivos de texto; ver inicializarDesdeArchivo.
    if (formatoCorrecto && validarSumaInicial) {
        int esperada = unidadesInicialesPorJugador(listaJugadores.longitud());
        for (int i = 0; i < listaJugadores.longitud(); ++i) {
            const std::string& color = listaJugadores.obtener(i).colorFicha;
            int suma = 0;
            for (int t = 0; t < tableroJuego.listaTerritoriosGlob.longitud(); ++t) {
                const Territorio& ter = tableroJuego.listaTerritoriosGlob.obtener(t);
                if (ter.colorPropietario == color) {
                    suma += ter.cantidadTropas;
                }
            }
            if (suma != esperada) {
                formatoCorrecto = false;
                break;
            }
        }
    }

    if (formatoCorrecto) {
        estadoJuego = ESTADO_EN_CURSO;
        turnoActualIndex = 0;
        // [IA] Generado con Claude (Prompt 13, Apendice A): si el primer jugador ya fue eliminado,
        // el turno empieza en el primer jugador que todavia tiene territorios.
        while (turnoActualIndex < listaJugadores.longitud() - 1 &&
               tableroJuego.cantidadTerritoriosOcupadosPor(listaJugadores.obtener(turnoActualIndex).colorFicha) == 0) {
            turnoActualIndex++;
        }
        nombreGanador = "";
        gruposCanjeados = 0;
    } else {
        listaJugadores.limpiar();
        tableroJuego.vaciarTablero();
    }
    return formatoCorrecto;
}

/*
===================================================================================
inicializarDesdeArchivo

Recibe: const std::string& nombreArchivo. Retorna: ResultadoInicializacion

Funcionamiento (Paso a paso):
1. Verifica que el archivo se pueda abrir (si no, ARCHIVO_ERRONEO) y que no este vacio
   (si esta vacio, ARCHIVO_VACIO).
2. Si el nombre termina en ".bin" intenta decodificarlo con Huffman (cabecera, arbol con las
   mismas frecuencias y w caracteres). Si lo logra, carga ese texto sin exigir la suma inicial,
   porque un .bin lo genera guardar_comprimido y puede tener una partida en curso.
3. Si no, lee el archivo como texto plano y lo carga exigiendo la suma inicial (35/30/25/20).
4. Si el nombre no termina en ".bin" y el texto no sirvio, intenta como comprimido.
5. Si nada funciono, SIN_FORMATO.
===================================================================================
*/
// Decodifica un archivo comprimido con Huffman; Recibe: const string& nombreArchivo, string& contenido; Retorna: bool
// Lee la cabecera (n, pares ci fi, w), reconstruye el arbol con esas frecuencias y decodifica
// exactamente w caracteres. Retorna false si la cabecera o el codigo no tienen el formato.
// [IA] Funcion extraida con Claude (Prompt 14, Apendice A) del codigo que antes estaba dentro de
// inicializarDesdeArchivo, para poder usarla tambien cuando la extension no es .bin.
static bool decodificarArchivoHuffman(const std::string& nombreArchivo, std::string& contenido) {
    ArchivoHuffman archivoBin(nombreArchivo);
    if (!archivoBin.leer()) {
        return false;
    }
    ArbolHuffman arbol;
    arbol.fijarFrecuencias(archivoBin.frecuencias);
    arbol.construirArbol();
    contenido = arbol.decodificar(archivoBin.desempaquetarBits(), archivoBin.w);
    return (unsigned long long)contenido.size() == archivoBin.w;
}

ResultadoInicializacion JuegoRisk::inicializarDesdeArchivo(const std::string& nombreArchivo) {
    // 1. El archivo debe existir y tener contenido
    std::ifstream prueba(nombreArchivo.c_str(), std::ios::in | std::ios::binary);
    if (!prueba.is_open()) {
        return INICIALIZACION_ARCHIVO_ERRONEO;
    }
    if (prueba.peek() == std::ifstream::traits_type::eof()) {
        return INICIALIZACION_ARCHIVO_VACIO;
    }
    prueba.close();

    // [IA] Pasos 2 y 3 reorganizados con Claude (Prompt 14, Apendice A): la extension decide que
    // formato se intenta primero, y si ese falla se intenta el otro. Asi un .bin generado por
    // guardar_comprimido con otro nombre, o un texto de guardar llamado .bin, tambien se cargan.
    // La suma inicial solo se exige al contenido que se leyo como texto plano.
    std::string decodificado;
    bool pareceComprimido = ArchivoHuffman::esArchivoComprimido(nombreArchivo);

    // 2. Extension .bin: primero como archivo comprimido con Huffman
    if (pareceComprimido && decodificarArchivoHuffman(nombreArchivo, decodificado)) {
        return cargarDesdeTexto(decodificado, false) ? INICIALIZACION_CORRECTA : INICIALIZACION_SIN_FORMATO;
    }

    // 3. Como texto plano (con la validacion de la suma inicial)
    std::ifstream archivo(nombreArchivo.c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << archivo.rdbuf();
    archivo.close();
    if (cargarDesdeTexto(buffer.str(), true)) {
        return INICIALIZACION_CORRECTA;
    }

    // 4. Otra extension: ultimo intento como archivo comprimido
    if (!pareceComprimido && decodificarArchivoHuffman(nombreArchivo, decodificado) &&
        cargarDesdeTexto(decodificado, false)) {
        return INICIALIZACION_CORRECTA;
    }
    return INICIALIZACION_SIN_FORMATO;
}

// Genera el texto del estado actual de la partida; Recibe: nada; Retorna: string
// Primero escribe la cantidad de jugadores, luego cada jugador ("nombre color") y por ultimo
// los 42 territorios del tablero en orden ("codigo color tropas").
std::string JuegoRisk::serializarEstado() const {
    std::ostringstream texto;
    texto << listaJugadores.longitud() << "\n";
    for (int i = 0; i < listaJugadores.longitud(); ++i) {
        texto << listaJugadores.obtener(i).nombreJugador << " "
              << listaJugadores.obtener(i).colorFicha << "\n";
    }
    for (int i = 0; i < tableroJuego.listaTerritoriosGlob.longitud(); ++i) {
        const Territorio& ter = tableroJuego.listaTerritoriosGlob.obtener(i);
        texto << ter.idTerritorio << " " << ter.colorPropietario << " "
              << ter.cantidadTropas << "\n";
    }
    return texto.str();
}

// Guarda el estado en un archivo de texto plano; Recibe: const string& nombreArchivo; Retorna: bool
// Abre el archivo, escribe el texto retornado por serializarEstado() y retorna true si no hubo errores.
bool JuegoRisk::guardarEnTexto(const std::string& nombreArchivo) const {
    std::ofstream archivo(nombreArchivo.c_str(), std::ios::out | std::ios::trunc);
    if (!archivo.is_open()) {
        return false;
    }
    archivo << serializarEstado();
    archivo.close();
    return !archivo.fail();
}

// Guarda el estado en un archivo binario comprimido con Huffman; Recibe: const string& nombreArchivo; Retorna: bool
// Serializa el estado a texto, calcula frecuencias, construye el arbol, genera codigos, codifica a bits y escribe el archivo .bin.
bool JuegoRisk::guardarEnComprimido(const std::string& nombreArchivo) const {
    std::string texto = serializarEstado();

    ArbolHuffman arbol;
    arbol.calcularFrecuencias(texto);
    arbol.construirArbol();
    if (arbol.esVacio()) {
        return false;
    }
    arbol.generarCodigos();
    std::string bits = arbol.codificar(texto);

    ArchivoHuffman archivoBin(nombreArchivo);
    return archivoBin.escribir(arbol, bits);
}
