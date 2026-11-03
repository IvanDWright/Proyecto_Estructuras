#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <vector>
#ifdef _WIN32
// [IA] Bloque _WIN32 (aqui y al inicio de main) generado con Claude (Prompt 12, Apendice A:
// "las tildes se ven distorsionadas en la consola de Windows").
// En Windows la consola usa por defecto una pagina de codigos distinta de UTF-8 (437 u 850)
// y las tildes de los mensajes se ven distorsionadas. Se cambia la consola a UTF-8 al iniciar.
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

// Inclusiones de los TADs del proyecto (Rutas relativas a la carpeta src)
#include "Carta.h"
#include "Continente.h"
#include "JuegoRisk.h"
#include "Jugador.h"
#include "ListaLineal.h"
#include "TableroRisk.h"
#include "Territorio.h"

using namespace std;

// Estructura para almacenar información de ayuda de comandos
struct ComandoAyuda {
  string nombre;
  string uso;
  string descripcion;
};

// Lista de comandos para el comando 'ayuda'
// [IA] Con Claude (Prompt 13, Apendice A) se cambio el vector con lista inicializadora (C++11)
// por un arreglo, y los "for (auto ...)" por ciclos con indice, para compilar con g++ en C++98.
const ComandoAyuda comandos[] = {
    {"inicializar", "inicializar <archivo>",
     "Inicializa el juego con los datos de un archivo de texto plano o binario."},
    {"obtener_unidades", "obtener_unidades <nombre_jugador>",
     "Calcula y asigna unidades adicionales al jugador."},
    {"atacar", "atacar <nombre_jugador>",
     "Inicia la fase de ataque para el jugador especificado."},
    {"fortificar", "fortificar <nombre_jugador>",
     "Inicia la fase de fortificacion para el jugador especificado."},
    {"estado_juego", "estado_juego", "Muestra el estado actual del juego."},
    {"salir", "salir", "Termina la ejecucion de la aplicacion."},
    {"guardar", "guardar <nombre_archivo>",
     "Guarda el estado del juego en un archivo de texto plano."},
    {"guardar_comprimido", "guardar_comprimido <nombre_archivo>",
     "Guarda el estado del juego en un archivo binario comprimido (Huffman)."},
    {"costo_conquista", "costo_conquista <nombre_jugador> <territorio>",
     "Calcula el costo y secuencia de territorios para conquistar el territorio indicado."},
    {"conquista_mas_barata", "conquista_mas_barata <nombre_jugador>",
     "Calcula y sugiere la conquista que implica la menor perdida de ejercitos."},
    {"ayuda", "ayuda [comando]",
     "Muestra la lista de comandos disponibles o detalla el uso de un comando especifico."}};
const int NUM_COMANDOS = sizeof(comandos) / sizeof(comandos[0]);

// Función auxiliar para imprimir la lista general de comandos
void mostrar_ayuda() {
  cout << "Lista de comandos disponibles:\n";
  for (int i = 0; i < NUM_COMANDOS; ++i) {
    cout << " - " << comandos[i].nombre << "\n";
  }
}

// Función auxiliar para imprimir el detalle de un comando
void mostrar_ayuda_comando(const string &cmd) {
  for (int i = 0; i < NUM_COMANDOS; ++i) {
    const ComandoAyuda &c = comandos[i];
    if (c.nombre == cmd) {
      cout << "Uso: " << c.uso << "\n";
      cout << "Descripcion: " << c.descripcion << "\n";
      return;
    }
  }
  cout << "El comando '" << cmd
       << "' no existe. Escriba 'ayuda' para ver los comandos disponibles.\n";
}

/*
======================================================================================
Funciones auxiliares de lectura de entrada por consola.
Se leen las respuestas de los comandos (territorios, cantidades, s/n) por lineas
completas para evitar inconsistencias en el buffer de entrada de cin.
======================================================================================
*/

// Lee una linea completa de la entrada estandar y elimina espacios iniciales y finales; Recibe: nada; Retorna: string
// Si se termina la entrada (EOF), finaliza la ejecucion limpiamente.
string leerLinea() {
  string linea;
  if (!getline(cin, linea)) {
    exit(0);
  }
  size_t inicio = linea.find_first_not_of(" \t\r\n");
  if (inicio == string::npos) {
    return "";
  }
  size_t fin = linea.find_last_not_of(" \t\r\n");
  return linea.substr(inicio, fin - inicio + 1);
}

// Lee una linea y la convierte a un numero entero; Recibe: int& valor; Retorna: bool
// Retorna true si la linea contiene exactamente un entero valido sin caracteres sobrantes.
bool leerEntero(int &valor) {
  string texto = leerLinea();
  stringstream ss(texto);
  char sobrante;
  return (ss >> valor) && !(ss >> sobrante);
}

// Busca un territorio en un vector segun su codigo oficial; Recibe: const vector<Territorio*>& lista, const string& codigo; Retorna: Territorio*
// Retorna el puntero al territorio encontrado o NULL si no existe en la lista.
Territorio *buscarTerritorioPorCodigo(const vector<Territorio *> &lista,
                                      const string &codigo) {
  for (size_t i = 0; i < lista.size(); ++i) {
    if (lista[i]->idTerritorio == codigo) {
      return lista[i];
    }
  }
  return NULL;
}

/*
======================================================================================
Combate interactivo. La logica (lanzar dados, ordenarlos, comparar y descontar bajas)
vive en el TAD JuegoRisk::resolverRondaCombate; esta funcion solo muestra los
resultados de cada ronda y pregunta si se desea continuar.
Si el defensor se queda sin tropas el atacante conquista el territorio. Si el atacante
se queda sin tropas o decide parar, el defensor mantiene el territorio.
======================================================================================
*/
bool atacar(JuegoRisk &juego, int &tropas_atacantes, int &tropas_defensores) {
  while (tropas_atacantes > 0 && tropas_defensores > 0) {
    vector<int> tiradas_atacante;
    vector<int> tiradas_defensor;

    // La ronda completa (dados y bajas) se resuelve dentro del TAD
    juego.resolverRondaCombate(tropas_atacantes, tropas_defensores,
                               tiradas_atacante, tiradas_defensor);

    cout << "\n[!] Lanzamiento de dados...\n";
    cout << "Atacante tira " << tiradas_atacante.size() << " dado(s):\n";
    for (size_t i = 0; i < tiradas_atacante.size(); ++i) {
      cout << "  - Dado " << i + 1 << ": " << tiradas_atacante[i] << "\n";
    }
    cout << "Defensor tira " << tiradas_defensor.size() << " dado(s):\n";
    for (size_t i = 0; i < tiradas_defensor.size(); ++i) {
      cout << "  - Dado " << i + 1 << ": " << tiradas_defensor[i] << "\n";
    }

    size_t duelos = min(tiradas_atacante.size(), tiradas_defensor.size());
    cout << "\n--- Resultados de los duelos ---\n";
    for (size_t i = 0; i < duelos; ++i) {
      cout << "Duelo " << i + 1 << ": Atacante (" << tiradas_atacante[i]
           << ") vs Defensor (" << tiradas_defensor[i] << ") -> ";
      if (tiradas_atacante[i] > tiradas_defensor[i]) {
        cout << "El defensor pierde 1 tropa.\n";
      } else {
        cout << "El atacante pierde 1 tropa.\n";
      }
    }
    cout << "--------------------------------\n";
    cout << "Tropas restantes de la invasion - Atacante: " << tropas_atacantes
         << " | Defensor: " << tropas_defensores << "\n\n";

    if (tropas_defensores == 0) {
      cout << "¡El atacante ha conquistado el territorio!\n";
      return true;
    }
    if (tropas_atacantes == 0) {
      cout << "El atacante se ha quedado sin tropas en esta invasion.\n";
      return false;
    }

    cout << "¿Deseas continuar el ataque con las tropas restantes? (s/n): ";
    // La respuesta se lee por linea completa
    string resp = leerLinea();
    if (resp != "s" && resp != "S") {
      break;
    }
  }

  cout << "El defensor mantiene el territorio por ahora.\n";
  return false;
}

// Cierra la fase de ataque del jugador; Recibe: JuegoRisk& juego, Jugador& jugador; Retorna: void
// Marca que ya ataco, avanza a la fase de fortificacion y, si conquisto algun territorio en el turno,
// le entrega la carta correspondiente (la logica de la carta esta en JuegoRisk).
void finalizarFaseAtaque(JuegoRisk &juego, Jugador &jugador) {
  if (juego.terminarFaseAtaque(jugador)) {
    cout << "[*] Recibiste una carta por conquistar territorios en este turno.\n";
  }
}

int main() {
#ifdef _WIN32
  // Consola en UTF-8 para que las tildes y los signos ¿ y ¡ se vean bien en Windows
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
#endif
  // Se inicializa la semilla una sola vez para que los dados cambien en cada ejecucion
  srand(static_cast<unsigned int>(time(NULL)));

  cout << "Bienvenido al juego Risk. Pon 'ayuda' para ver los comandos disponibles.\n";

  string linea;
  bool is_running = true;
  bool is_initialized = false;
  JuegoRisk juego;

  // Ciclo de evaluación de la consola interactiva (REPL)
  while (is_running) {
    // Imprime estrictamente el caracter $ como indicador de linea
    cout << "$ ";
    if (!getline(cin, linea)) {
      break; // Finalizar si ocurre EOF
    }

    if (linea.empty())
      continue;

    // Tokenización básica por espacios
    stringstream ss(linea);
    string comando;
    vector<string> args;

    ss >> comando;
    string arg;
    while (ss >> arg) {
      args.push_back(arg);
    }

    // ==========================================
    // COMANDO: inicializar
    // ==========================================
    /*
    ===================================================================================
    Inicializa el juego tanto desde un archivo de texto plano (generado por 'guardar' o
    escrito a mano) como desde un archivo binario comprimido con Huffman (generado por
    'guardar_comprimido', termina en ".bin").
    La lectura y validacion del archivo se delega a JuegoRisk::inicializarDesdeArchivo.
    ===================================================================================
    */
    if (comando == "inicializar") {
      if (args.size() == 1) {
        if (is_initialized) {
          cout << "(Juego en curso) El juego ya ha sido inicializado.\n";
        } else {
          // Lectura y carga del archivo delegada al TAD JuegoRisk
          ResultadoInicializacion resultado =
              juego.inicializarDesdeArchivo(args[0]);

          if (resultado == INICIALIZACION_ARCHIVO_ERRONEO) {
            cout << "(Archivo erróneo) " << args[0]
                 << " no se encuentra o no puede leerse.\n";
          } else if (resultado == INICIALIZACION_ARCHIVO_VACIO) {
            cout << "(Archivo vacío) " << args[0]
                 << " no contiene información.\n";
          } else if (resultado == INICIALIZACION_SIN_FORMATO) {
            cout << "(Archivo sin formato) " << args[0]
                 << " no contiene información en el formato esperado.\n";
          } else {
            is_initialized = true;
            cout << "(Comando correcto) El juego se ha inicializado "
                    "correctamente.\n";
          }
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: obtener_unidades
    // ==========================================
    /*
    =====================================================================================
    verificaciones:
    1. El juego esta inicializado y no ha terminado.
    2. El jugador existe y es su turno.
    3. El jugador aun no ha ejecutado obtener_unidades en este turno.
    4. La seleccion es uno de sus territorios

    Funcionamiento: 
    1. JuegoRisk calcula las unidades: max(3, territorios / 3) + bono de continentes.
    2. Si el jugador tiene un grupo valido de 3 cartas se le pregunta si desea canjearlo
       (la deteccion y el canje los hacen Jugador y JuegoRisk; aqui solo se pregunta).
    3. Muestra los territorios del jugador con su codigo oficial, se elige digitando el codigo
       y la cantidad de tropas a ubicar, repitiendo hasta agotar las unidades por asignar.
    =======================================================================================
    */
    else if (comando == "obtener_unidades") {
      // Valida cantidad de parámetros
      if (args.size() == 1) {
        // Validaciones de estado global
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.estaTerminado()) {
          cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n";
        }
        // Validaciones del jugador y turnos
        else if (juego.buscarJugadorPorNombre(args[0]) == NULL) {
          cout << "(Jugador no válido) El jugador " << args[0]
               << " no forma parte de esta partida.\n";
        } else if (juego.obtenerJugadorEnTurno()->nombreJugador != args[0]) {
          cout << "(Jugador fuera de turno) No es el turno del jugador "
               << args[0] << ".\n";
        } else if (juego.obtenerJugadorEnTurno()->faseTurnoActual !=
                   FASE_OBTENER_UNIDADES) {
          // Evita reclamar unidades mas de una vez en el mismo turno
          cout << "(Jugador ya obtuvo unidades) El jugador " << args[0]
               << " ya ejecuto obtener_unidades en este turno.\n";
        } else {
          Jugador *jugadorActual = juego.obtenerJugadorEnTurno();

          // Obtiene los territorios del jugador actual
          vector<Territorio *> territorio_posible;
          juego.tableroJuego.territoriosDeJugador(jugadorActual->colorFicha,
                                                  territorio_posible);

          // Unidades base: territorios / 3 + bono de continentes (calculo en JuegoRisk)
          int tropas_a_anadir = juego.calcularUnidadesBase(*jugadorActual);

          // Canje de cartas: JuegoRisk/Jugador detectan y aplican el canje;
          // aqui solo se pregunta al usuario si desea hacerlo.
          int i1 = -1, i2 = -1, i3 = -1;
          while (jugadorActual->buscarGrupoCanjeable(i1, i2, i3)) {
            cout << "\nTienes un grupo valido de cartas para intercambiar. "
                    "¿Deseas intercambiarlas por tropas? (s/n): ";
            string resp = leerLinea();
            if (resp != "s" && resp != "S") {
              break;
            }
            // [IA] Bloque modificado con Claude (Prompt 13, Apendice A): ahora se informa
            // el +2 de cada territorio propio que aparezca en las cartas canjeadas.
            ListaLineal<string> territoriosBono;
            int bono = juego.canjearCartas(*jugadorActual, i1, i2, i3,
                                           territoriosBono);
            tropas_a_anadir += bono;
            cout << "Has canjeado un grupo de cartas por " << bono
                 << " unidades.\n";
            for (int b = 0; b < territoriosBono.longitud(); ++b) {
              cout << "¡Has recibido +2 unidades en el territorio "
                   << territoriosBono.obtener(b)
                   << " por poseer la carta de ese territorio!\n";
            }
          }

          // Si el jugador no tiene territorios no hay donde ubicar unidades
          if (territorio_posible.empty()) {
            tropas_a_anadir = 0;
          }

          // [IA] Mensaje agregado con Claude (Prompt 14, Apendice A): sin el minimo de 3, un jugador
          // con menos de 3 territorios y sin continentes recibe 0 unidades; el enunciado pide
          // informar primero cuantas unidades puede reclamar, tambien en ese caso.
          if (tropas_a_anadir == 0) {
            cout << "\nTienes 0 unidades para ubicar en este turno.\n";
          }

          while (tropas_a_anadir > 0) {
            cout << "\nTienes " << tropas_a_anadir
                 << " unidades para ubicar.\n";
            cout << "En cual de tus territorios deseas ubicar unidades?\n";

            // Se muestra el codigo de cada territorio del jugador
            for (vector<Territorio *>::iterator i = territorio_posible.begin();
                 i != territorio_posible.end(); ++i) {
              cout << " " << (*i)->idTerritorio << " - " << (*i)->nombreTerritorio
                   << " (Actuales: " << (*i)->cantidadTropas << ")\n";
            }

            // Selecciona el territorio por su codigo oficial
            cout << "Ingresa el codigo del territorio (ej. "
                 << territorio_posible[0]->idTerritorio << "): ";
            string codigo = leerLinea();
            Territorio *destino =
                buscarTerritorioPorCodigo(territorio_posible, codigo);

            if (destino != NULL) {
              int cantidad = 0;
              cout << "Cuantas unidades deseas ubicar aqui? (Maximo "
                   << tropas_a_anadir << "): ";

              // [IA] Llamada a ubicarUnidades generada con Claude (Prompt 12, Apendice A).
              // JuegoRisk valida la cantidad (1 a las unidades por ubicar) y la suma al territorio
              if (leerEntero(cantidad) &&
                  juego.ubicarUnidades(*jugadorActual, destino, cantidad,
                                       tropas_a_anadir)) {
                tropas_a_anadir -= cantidad;

                cout << "-> Se han anadido " << cantidad << " tropas a "
                     << destino->nombreTerritorio << ".\n";
              } else {
                cout << "(Cantidad invalida, intenta de nuevo)\n";
              }
            } else {
              cout << "(Codigo invalido, intenta de nuevo)\n";
            }
          }

          jugadorActual->avanzarFase(FASE_ATACAR); // Avanza a la siguiente fase
          cout << "(Comando correcto) El jugador " << args[0]
               << " ha terminado de reclamar y ubicar sus unidades.\n";
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: atacar
    // ==========================================

    /*
    ===================================================================================
    validaciones globales:
    1. El juego esta en curso y no ha terminado.
    2. El jugador existe, es su turno y ya ha reclamado sus unidades.
    3. verifica que el jugador ya ataco mediante el bood de "haAtacado"

    Funcionamiento (Paso a paso):
    1. Obtiene TODOS los territorios del jugador y filtra los que pueden atacar
       (Mas de 1 tropa y tenga vecinos enemigos). Si no hay territorios válidos, informa de 
       inmediato y finaliza la acción.
    3. Selección de origen: Muestra TODOS los territorios del jugador con su codigo (indicando 
       cuales no estan disponibles para atacar) y pide al jugador escribir el CODIGO del 
       territorio desde cual atacar, junto con la cantidad de tropas a utilizar (las cuales son 
       temporalmente restadas del territorio de origen).
    4. Filtrado de objetivo: Una vez elegido el origen, crea un vector únicamente con los 
       territorios vecinos que pertenecen a otros jugadores, y pide escribir el CODIGO del 
       territorio a atacar.
    5. Ejecución del ataque: Llama a la función 'atacar()' pasándole las tropas atacantes 
       y defensoras. Debido a que la función recibe estos parámetros por referencia (int &), 
       las bajas se aplican y actualizan directamente en las variables suministradas.
    6. Resultados: Si el ataque es exitoso, se asigna el territorio al atacante y se 
       ubican las tropas sobrevivientes allí (y se verifica si ya conquisto los 42 territorios).
       Si fracasa, las tropas sobrevivientes regresan al territorio de origen.
    Muestra todos los territorios del jugador con su codigo oficial y permite seleccionar origen
    y destino por codigo oficial.
    ===================================================================================
    */
    else if (comando == "atacar") {
      if (args.size() == 1) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.estaTerminado()) {
          cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n";
        } else if (juego.buscarJugadorPorNombre(args[0]) == NULL) {
          cout << "(Jugador no válido) El jugador " << args[0]
               << " no forma parte de esta partida.\n";
        } else if (juego.obtenerJugadorEnTurno()->nombreJugador != args[0]) {
          cout << "(Jugador fuera de turno) No es el turno del jugador "
               << args[0] << ".\n";
        } else if (juego.obtenerJugadorEnTurno()->faseTurnoActual ==
                    FASE_OBTENER_UNIDADES) {
          // Valida que el jugador ya haya obtenido sus unidades
          cout << "(Jugador no ha ubicado unidades) El jugador " << args[0]
               << " no ha ejecutado el comando obtener_unidades.\n";
        } else if (juego.obtenerJugadorEnTurno()->faseTurnoActual ==
                    FASE_FORTIFICAR) {
          // Valida que el jugador ya haya fortificado
          cout << "(Jugador ya fortifico) El jugador " << args[0]
               << " ya no puede atacar.\n";
        } else {
          Jugador *jugadorActual = juego.obtenerJugadorEnTurno();

          // Territorios del jugador: todos (para listarlos) y los aptos para atacar
          // (mas de 1 tropa y vecino enemigo), ambos calculados por TableroRisk
          vector<Territorio *> todosMisTerritorios;
          vector<Territorio *> misTerritorios;
          juego.tableroJuego.territoriosDeJugador(jugadorActual->colorFicha,
                                                  todosMisTerritorios);
          juego.tableroJuego.territoriosAptosParaAtacar(
              jugadorActual->colorFicha, misTerritorios);

          if (misTerritorios.empty()) {
            cout << "(Jugador no puede atacar) " << args[0]
                 << " no tiene territorios con mas de 1 tropa y vecinos enemigos para atacar.\n";
            // Si no puede atacar, avanza a fortificar para no bloquear el turno del jugador
            finalizarFaseAtaque(juego, *jugadorActual);
          } else {
            // Muestra todos los territorios del jugador con su codigo oficial
            cout << "\nDesde cual de tus territorios quieres atacar? (Ingresa 0 para no atacar / terminar fase de ataque)\n";
            for (size_t i = 0; i < todosMisTerritorios.size(); ++i) {
              Territorio *t = todosMisTerritorios[i];
              cout << " " << t->idTerritorio << " - " << t->nombreTerritorio
                   << " (tienes " << t->cantidadTropas << " tropas)";
              if (buscarTerritorioPorCodigo(misTerritorios, t->idTerritorio) == NULL) {
                if (t->cantidadTropas <= 1) {
                  cout << " [No disponible: necesita mas de 1 tropa]";
                } else {
                  cout << " [No disponible: no tiene vecinos enemigos]";
                }
              }
              cout << "\n";
            }
            cout << " 0 - Terminar fase de ataque y pasar a fortificar\n";

            // Se escoge el territorio de origen por su codigo oficial
            Territorio *territorioOrigen = NULL;
            bool terminarAtaque = false;
            while (territorioOrigen == NULL && !terminarAtaque) {
              cout << "Ingresa el codigo del territorio de origen (o 0 para terminar): ";
              string codigo = leerLinea();
              if (codigo == "0") {
                terminarAtaque = true;
              } else {
                territorioOrigen = buscarTerritorioPorCodigo(misTerritorios, codigo);
                if (territorioOrigen == NULL) {
                  cout << "Error: Codigo invalido o territorio no disponible para atacar\n";
                }
              }
            }

            if (terminarAtaque) {
              finalizarFaseAtaque(juego, *jugadorActual);
              cout << "(Comando correcto) El jugador " << args[0]
                   << " ha terminado de atacar.\n";
            } else {
              int maxTropas = territorioOrigen->cantidadTropas - 1;

              cout << "Con cuantas tropas quieres atacar? (1 a " << maxTropas << "): \n";
              int tropas = 0;
              // [IA] Validacion y mensaje de error generados con Claude (Prompt 12, Apendice A:
              // "corrige el mensaje de finalizacion correcta despues de una cantidad invalida").
              // JuegoRisk valida la cantidad (1 a tropas del origen - 1) y la aparta del origen.
              // Si no es valida no se modifica nada: el comando termina sin el mensaje de
              // exito y el jugador sigue en la fase de ataque (puede volver a ejecutar atacar).
              if (!leerEntero(tropas) ||
                  !juego.apartarTropasAtacantes(*jugadorActual, territorioOrigen, tropas)) {
                cout << "Error: Opcion invalida. El ataque no se realizo; puedes ejecutar atacar de nuevo.\n";
              } else {

                // Filtrar únicamente los vecinos que pertenecen a otros jugadores
                vector<Territorio *> territoriosAtacar;
                for (int i = 0; i < territorioOrigen->adyacentes.longitud(); ++i) {
                  Territorio *vecino = territorioOrigen->adyacentes.obtener(i);
                  if (vecino->colorPropietario != jugadorActual->colorFicha) {
                    territoriosAtacar.push_back(vecino);
                  }
                }

                // Se muestra el codigo de cada territorio objetivo
                cout << "\nA cual territorio quieres atacar?\n";
                for (size_t i = 0; i < territoriosAtacar.size(); ++i) {
                  cout << " " << territoriosAtacar[i]->idTerritorio << " - "
                       << territoriosAtacar[i]->nombreTerritorio
                       << " (Propietario: " << territoriosAtacar[i]->colorPropietario
                       << " - Tropas: " << territoriosAtacar[i]->cantidadTropas << ")\n";
                }

                // El territorio a atacar se escoge por su codigo oficial
                Territorio *territorioDefensor = NULL;
                while (territorioDefensor == NULL) {
                  cout << "Ingresa el codigo del territorio a atacar: ";
                  string codigo = leerLinea();
                  territorioDefensor = buscarTerritorioPorCodigo(territoriosAtacar, codigo);
                  if (territorioDefensor == NULL) {
                    cout << "Error: Codigo invalido\n";
                  }
                }

                // [IA] Hasta el mensaje "(Comando correcto)" el bloque se reorganizo con Claude
                // (Prompt 12, Apendice A) para que main no modifique directamente los territorios.
                // El combate trabaja sobre una copia de las tropas del defensor; al final
                // JuegoRisk actualiza el territorio (conquista o tropas restantes)
                int tropasDefensor = territorioDefensor->cantidadTropas;
                bool gano = atacar(juego, tropas, tropasDefensor);

                if (gano) {
                  // JuegoRisk asigna el territorio, marca la conquista del turno y
                  // verifica si el jugador ya ocupa los 42 territorios (victoria)
                  bool victoria = juego.conquistarTerritorio(*jugadorActual, territorioDefensor, tropas);
                  cout << "¡Has conquistado el territorio " << territorioDefensor->nombreTerritorio << "!\n";
                  cout << "Tropas ubicadas en ese territorio: " << tropas << "\n";
                  if (victoria) {
                    cout << "¡El jugador " << juego.nombreGanador
                         << " ha conquistado todos los territorios y gana la partida!\n";
                  }
                } else {
                  juego.actualizarDefensor(territorioDefensor, tropasDefensor);
                  juego.devolverTropasAtacantes(territorioOrigen, tropas);
                  cout << "Ataque finalizado. Tropas sobrevivientes devueltas a tu territorio.\n";
                }
                // Habilita el comando fortificar (el ataque ya se ejecuto en este turno)
                jugadorActual->registrarAtaque();
                cout << "(Comando correcto) El jugador " << args[0]
                     << " ha terminado de atacar.\n";
              }
            }
          }
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    /*
    ===================================================================================
    verificaciones globales:
    1. El juego esta inicializado.
    2. El juego no ha terminado.
    3. El jugador existe, es su turno y ya ha atacado.

    Funcionamiento (Paso a paso):
    1. Verifica que el jugador ya haya pasado por la fase de ataque (jugadorHaAtacado).
    2. Pregunta al usuario si desea fortificar un territorio.
    3. Muestra una lista (con codigos) de territorios propios que tienen al menos un vecino propio y >1 tropa.
    4. Pide escribir el CODIGO de un territorio de origen y luego el CODIGO de un vecino propio como destino.
    5. Solicita la cantidad de tropas a mover (respetando que debe quedar al menos 1).
    6. Realiza el movimiento y pasa el turno al siguiente jugador.
    Los territorios se escogen por su codigo oficial.
    ===================================================================================
    */
    else if (comando == "fortificar") {
      if (args.size() == 1) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.estaTerminado()) {
          cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n";
        } else if (juego.buscarJugadorPorNombre(args[0]) == NULL) {
          cout << "(Jugador no válido) El jugador " << args[0]
               << " no forma parte de esta partida.\n";
        } else if (juego.obtenerJugadorEnTurno()->nombreJugador != args[0]) {
          cout << "(Jugador fuera de turno) No es el turno del jugador "
               << args[0] << ".\n";
        } else if (!juego.obtenerJugadorEnTurno()->haAtacadoTurnoActual) {
          // Valida que el jugador haya pasado por la fase de ataque primero
          cout << "(Jugador no ha atacado) El jugador " << args[0]
               << " no ha ejecutado el comando atacar.\n";
        } else {
          Jugador *jugadorActual = juego.obtenerJugadorEnTurno();

          // Termina la fase de ataque: pasa a Fortificar y, si conquisto algun territorio, recibe su carta
          if (juego.terminarFaseAtaque(*jugadorActual)) {
            cout << "[*] Recibiste una carta por conquistar territorios en este turno.\n";
          }

          vector<Territorio *> territoriosDisponibles;
          vector<Territorio *> vecinosDisponibles;
          Territorio *a = NULL;
          Territorio *b = NULL;

          cout << "¿Quieres fortificar algun territorio? (s/n): ";
          // La respuesta se lee por linea completa
          string eleccion = leerLinea();
          if (eleccion == "s" || eleccion == "S") {
            // TableroRisk entrega los territorios con mas de 1 tropa y vecino propio
            juego.tableroJuego.territoriosAptosParaFortificar(
                jugadorActual->colorFicha, territoriosDisponibles);

            if (territoriosDisponibles.empty()) {
              cout << "No tienes territorios con tropas suficientes y vecinos propios para fortificar.\n";
            } else {
              // Se muestra el codigo de cada territorio y se escoge por codigo oficial
              cout << "\nDesde que territorio quieres mover unidades?\n";
              for (size_t i = 0; i < territoriosDisponibles.size(); i++) {
                cout << " " << territoriosDisponibles[i]->idTerritorio << " - "
                     << territoriosDisponibles[i]->nombreTerritorio << " ("
                     << territoriosDisponibles[i]->cantidadTropas << " tropas)\n";
              }

              while (a == NULL) {
                cout << "Ingresa el codigo del territorio de origen: ";
                a = buscarTerritorioPorCodigo(territoriosDisponibles, leerLinea());
                if (a == NULL) {
                  cout << "Error: Codigo invalido. Intenta de nuevo.\n";
                }
              }

              cout << "\nHacia que vecino quieres fortificar?\n";
              for (int i = 0; i < a->adyacentes.longitud(); i++) {
                if (a->adyacentes.obtener(i)->colorPropietario == jugadorActual->colorFicha) {
                  vecinosDisponibles.push_back(a->adyacentes.obtener(i));
                }
              }

              for (size_t i = 0; i < vecinosDisponibles.size(); i++) {
                cout << " " << vecinosDisponibles[i]->idTerritorio << " - "
                     << vecinosDisponibles[i]->nombreTerritorio << " ("
                     << vecinosDisponibles[i]->cantidadTropas << " tropas)\n";
              }

              while (b == NULL) {
                cout << "Ingresa el codigo del territorio destino: ";
                b = buscarTerritorioPorCodigo(vecinosDisponibles, leerLinea());
                if (b == NULL) {
                  cout << "Error: Codigo invalido. Intenta de nuevo.\n";
                }
              }

              int maxTropas = a->cantidadTropas - 1;
              int num = 0;
              cout << "¿Cuantas tropas deseas mover? (1 a " << maxTropas << "): ";
              while (!leerEntero(num) || num <= 0 || num > maxTropas) {
                cout << "Error: Opcion invalida. Intenta de nuevo: ";
              }

              // El movimiento (con sus validaciones) lo realiza JuegoRisk
              if (juego.moverTropas(*jugadorActual, a, b, num)) {
                cout << "Has movido " << num << " tropas de " << a->nombreTerritorio << " a " << b->nombreTerritorio << ".\n";
              } else {
                cout << "No se pudo realizar el movimiento de tropas.\n";
              }
            }
          }
          
          cout << "(Comando correcto) El jugador " << args[0]
               << " ha terminado de fortificar su posición.\n";
          // Al terminar la fortificación, finaliza el turno del jugador actual
          juego.pasarAlSiguienteTurno();
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: estado_juego
    // ==========================================
    /*
    ===================================================================================
    verificaciones globales:
    1. El juego esta inicializado.
    2. El juego no ha terminado.

    Funcionamiento (Paso a paso):
    1. Imprime el numero de jugadores, nombres y colores de cada uno.
    2. Imprime el jugador con el turno actual.
    3. Imprime la lista de los territorios, mostrando el color del jugador que lo controla
       y la cantidad de unidades que hay ubicadas en cada uno.
    ===================================================================================
    */
    else if (comando == "estado_juego") {
      if (args.empty()) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.estaTerminado()) {
          cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n";
        } else {
          // Imprime el resumen de la partida en curso
          cout << "--- ESTADO DEL JUEGO ---\n";
          cout << "Jugadores en partida (" << juego.listaJugadores.longitud()
               << "):\n";
          for (int i = 0; i < juego.listaJugadores.longitud(); ++i) {
            Jugador &jug = juego.listaJugadores.obtener(i);
            cout << " - " << jug.nombreJugador << " [Color: " << jug.colorFicha << "]\n";
          }

          Jugador *enTurno = juego.obtenerJugadorEnTurno();
          if (enTurno != NULL) {
            cout << "\nJugador con el turno actual: " << enTurno->nombreJugador
                 << "\n\n";
          }

          cout << "Lista de Territorios:\n";
          for (int i = 0; i < juego.tableroJuego.listaTerritoriosGlob.longitud(); ++i) {
            Territorio &ter = juego.tableroJuego.listaTerritoriosGlob.obtener(i);
            cout << " - " << ter.idTerritorio << " (" << ter.nombreTerritorio << "): "
                 << "Controlado por '" << ter.colorPropietario << "' con "
                 << ter.cantidadTropas << " unidades.\n";
          }
          cout << "------------------------\n";
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: salir
    // ==========================================
    /*
    ===================================================================================
    verificaciones globales:
    - No requiere validaciones de estado de juego (siempre debe poder salir).

    Funcionamiento (Paso a paso):
    1. Establece la bandera is_running en false.
    2. Finaliza la ejecucion del ciclo REPL y de la aplicacion.
    ===================================================================================
    */
    else if (comando == "salir") {
      if (args.empty()) {
        is_running = false; // Finaliza la bandera de ejecución del REPL
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: guardar
    // ==========================================
    /*
    ===================================================================================
    verificaciones globales:
    1. El juego esta inicializado.

    Funcionamiento (Paso a paso):
    1. Guarda el estado actual del juego en un archivo de texto con el mismo formato
       utilizado en la inicializacion (JuegoRisk::guardarEnTexto).
    2. Si no se pudo escribir el archivo informa el error.
    ===================================================================================
    */
    else if (comando == "guardar") {
      if (args.size() == 1) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.guardarEnTexto(args[0])) {
          // Serialización del estado del juego en archivo de texto
          cout << "(Comando correcto) La partida ha sido guardada "
                  "correctamente.\n";
        } else {
          cout << "(Error al guardar) La partida no ha sido guardada "
                  "correctamente.\n";
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: guardar_comprimido
    // ==========================================
    /*
    ===================================================================================
    verificaciones globales:
    1. El juego esta inicializado.

    Funcionamiento (Paso a paso):
    1. Guarda el estado actual del juego utilizando el algoritmo de codificacion de 
       Huffman en un archivo binario (JuegoRisk::guardarEnComprimido).
    2. Si no se pudo codificar o escribir el archivo informa el error.
    ===================================================================================
    */
    else if (comando == "guardar_comprimido") {
      if (args.size() == 1) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.guardarEnComprimido(args[0])) {
          // Compresión mediante Huffman
          cout << "(Comando correcto) La partida ha sido codificada y guardada "
                  "correctamente.\n";
        } else {
          cout << "(Error al codificar y/o guardar) La partida no ha sido "
                  "codificada ni guardada correctamente.\n";
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: costo_conquista
    // ==========================================
    /*
    ===================================================================================
    verificaciones globales:
    1. El juego esta inicializado.
    2. El juego no ha terminado.

    Funcionamiento (Paso a paso):
    (Para la Entrega 3)
    1. Calcula el costo y la secuencia de territorios a conquistar utilizando algoritmos
       sobre grafos (como Dijkstra).
    2. Indica el camino mas barato para conquistar el territorio indicado por el jugador.
    ===================================================================================
    */
    else if (comando == "costo_conquista") {
      // Valida 2 parámetros: jugador y territorio
      if (args.size() == 2) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.estaTerminado()) {
          cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n";
        } else {
          // TODO: Búsqueda en grafos (Dijkstra u otro)
          cout << "(Comando correcto) Para conquistar el territorio " << args[1]
               << ", " << args[0]
               << " debe atacar desde territorio_1, pasando por los "
                  "territorios territorio_2, territorio_3, ..., territorio_m. "
                  "Debe conquistar n unidades de ejército.\n";
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: conquista_mas_barata
    // ==========================================
    /*
    ===================================================================================
    verificaciones globales:
    1. El juego esta inicializado.
    2. El juego no ha terminado.

    Funcionamiento (Paso a paso):
    (Para la Entrega 3)
    1. Calcula la ruta de conquista que implique el menor numero de perdidas de ejercitos,
       verificando todos los territorios posibles utilizando algoritmos sobre grafos.
    ===================================================================================
    */
    else if (comando == "conquista_mas_barata") {
      if (args.size() == 1) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.estaTerminado()) {
          cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n";
        } else {
          // TODO: Algoritmo completo de rutas sobre grafos
          cout << "(Comando correcto) La conquista más barata es avanzar sobre "
                  "el territorio territorio_1 desde el territorio "
                  "territorio_2. Para conquistar el territorio territorio_1, "
                  "debe atacar desde territorio_2, pasando por los territorios "
                  "territorio_3, territorio_4, ..., territorio_m. Debe "
                  "conquistar n unidades de ejército.\n";
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: ayuda
    // ==========================================
    /*
    ===================================================================================
    verificaciones globales:
    - No requiere validaciones de estado de juego (siempre debe mostrar ayuda).

    Funcionamiento (Paso a paso):
    1. Si no hay argumentos, llama a mostrar_ayuda() para listar todos los comandos.
    2. Si hay un argumento, llama a mostrar_ayuda_comando(comando) para mostrar 
       el uso y descripcion especifica de dicho comando.
    ===================================================================================
    */
    else if (comando == "ayuda") {
      if (args.empty()) {
        mostrar_ayuda();
      } else if (args.size() == 1) {
        mostrar_ayuda_comando(args[0]);
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO NO RECONOCIDO
    // ==========================================
    else {
      cout << "Comando no reconocido. Escriba 'ayuda' para ver la lista de "
              "comandos disponibles.\n";
    }
  }

  return 0;
}
