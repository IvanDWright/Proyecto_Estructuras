#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Inclusiones de los TADs del proyecto (Rutas relativas a la carpeta src)
#include "../tads/Carta.h"
#include "../tads/Continente.h"
#include "../tads/JuegoRisk.h"
#include "../tads/Jugador.h"
#include "../tads/ListaLineal.h"
#include "../tads/TableroRisk.h"
#include "../tads/Territorio.h"

using namespace std;

// Estructura para almacenar información de ayuda de comandos
struct ComandoAyuda {
  string nombre;
  string uso;
  string descripcion;
};

// Lista de comandos para el comando 'ayuda'
vector<ComandoAyuda> comandos = {
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

// Función auxiliar para imprimir la lista general de comandos
void mostrar_ayuda() {
  cout << "Lista de comandos disponibles:\n";
  for (const auto &cmd : comandos) {
    cout << " - " << cmd.nombre << "\n";
  }
}

int calcular_tropas(int a) {
  int total_tropas = 0;
  total_tropas += a / 3;
  return total_tropas;
}

// Función auxiliar para imprimir el detalle de un comando
void mostrar_ayuda_comando(const string &cmd) {
  for (const auto &c : comandos) {
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
Logica de ataque, se definen la cantidad de dados a utilizar segun la cantidad
de tropas. Se lanza un dado por cada dado a utilizar, se comparan los resultados
de mayor a menor. Si el dado del atacante es mayor al del defensor, el defensor
pierde 1 tropa. Si el dado del defensor es mayor o igual al del atacante, el
atacante pierde 1 tropa.
======================================================================================
*/
bool atacar(int &tropas_atacantes, int &tropas_defensores) {
  while (tropas_atacantes > 0 && tropas_defensores > 0) {
    int dados_atacante = min(3, tropas_atacantes);
    int dados_defensor = min(2, tropas_defensores);

    vector<int> tiradas_atacante(dados_atacante);
    vector<int> tiradas_defensor(dados_defensor);

    cout << "\n[!] Lanzamiento de dados...\n";
    cout << "Atacante tira " << dados_atacante << " dado(s):\n";
    for (int i = 0; i < dados_atacante; ++i) {
      tiradas_atacante[i] = rand() % 6 + 1;
      cout << "  - Dado " << i + 1 << ": " << tiradas_atacante[i] << "\n";
    }

    cout << "Defensor tira " << dados_defensor << " dado(s):\n";
    for (int i = 0; i < dados_defensor; ++i) {
      tiradas_defensor[i] = rand() % 6 + 1;
      cout << "  - Dado " << i + 1 << ": " << tiradas_defensor[i] << "\n";
    }

    sort(tiradas_atacante.rbegin(), tiradas_atacante.rend());
    sort(tiradas_defensor.rbegin(), tiradas_defensor.rend());

    int duelos = min(dados_atacante, dados_defensor);
    cout << "\n--- Resultados de los duelos ---\n";

    for (int i = 0; i < duelos; ++i) {
      cout << "Duelo " << i + 1 << ": Atacante (" << tiradas_atacante[i]
           << ") vs Defensor (" << tiradas_defensor[i] << ") -> ";
      if (tiradas_atacante[i] > tiradas_defensor[i]) {
        tropas_defensores--;
        cout << "El defensor pierde 1 tropa.\n";
      } else {
        tropas_atacantes--;
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
    string resp;
    cin >> resp;
    if (resp != "s" && resp != "S") {
      break;
    }
  }

  cout << "El defensor mantiene el territorio por ahora.\n";
  return false;
}

int main() {
  string linea;
  bool is_running = true;
  bool is_initialized = false;
  JuegoRisk juego;
  bool jugadorHaAtacado = false;

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
    if (comando == "inicializar") {
      if (args.size() == 1) {
        if (is_initialized) {
          cout << "(Juego en curso) El juego ya ha sido inicializado.\n";
        } else {
          ifstream archivo(args[0], ios::in);
          if (!archivo.is_open()) {
            cout << "(Archivo erróneo) " << args[0]
                 << " no se encuentra o no puede leerse.\n";
          } else if (archivo.peek() == ifstream::traits_type::eof()) {
            cout << "(Archivo vacío) " << args[0]
                 << " no contiene información.\n";
            archivo.close();
          } else {
            bool formatoCorrecto = true;
            int numJugadores;

            // 1. Leer número de jugadores
            if (!(archivo >> numJugadores) || numJugadores < 3 ||
                numJugadores > 6) {
              formatoCorrecto = false;
            }

            juego.jugadores.limpiar();
            juego.tablero.limpiar();
            juego.estado = NO_INICIALIZADO;

            struct TropasColor {
              string color;
              int tropas;
            };
            vector<TropasColor> tropasPorColor;

            // 2. Leer jugadores
            if (formatoCorrecto) {
              for (int i = 0; i < numJugadores; ++i) {
                string nombre, color;
                if (!(archivo >> nombre >> color)) {
                  formatoCorrecto = false;
                  break;
                }
                juego.jugadores.agregarAlFinal(Jugador(nombre, color));
                tropasPorColor.push_back({color, 0});
              }
            }

            // 3. Inicializar mapa y 4. Asignar territorios
            if (formatoCorrecto) {
              juego.tablero.inicializarMapa();
              for (int i = 0; i < 42; ++i) {
                string codTer, colorOcupante;
                int numTropas;
                if (!(archivo >> codTer >> colorOcupante >> numTropas)) {
                  formatoCorrecto = false;
                  break;
                }

                TropasColor *tcOcupante = nullptr;
                for (auto &tc : tropasPorColor) {
                  if (tc.color == colorOcupante) {
                    tcOcupante = &tc;
                    break;
                  }
                }

                if (tcOcupante == nullptr) {
                  formatoCorrecto =
                      false; // Color no pertenece a un jugador válido
                  break;
                }

                Territorio *ter = juego.tablero.buscarTerritorio(codTer);
                if (ter == nullptr) {
                  formatoCorrecto = false; // Territorio no existe
                  break;
                }

                ter->asignarOcupante(colorOcupante, numTropas);
                tcOcupante->tropas += numTropas;
              }
            }

            // 5. Validar reglas de Risk (Suma de tropas)
            if (formatoCorrecto) {
              int tropasEsperadas = 0;
              if (numJugadores == 3)
                tropasEsperadas = 35;
              else if (numJugadores == 4)
                tropasEsperadas = 30;
              else if (numJugadores == 5)
                tropasEsperadas = 25;
              else if (numJugadores == 6)
                tropasEsperadas = 20;

              for (const auto &tc : tropasPorColor) {
                if (tc.tropas != tropasEsperadas) {
                  formatoCorrecto = false;
                  break;
                }
              }
            }

            archivo.close();

            if (formatoCorrecto) {
              is_initialized = true;
              juego.estado = EN_CURSO;
              juego.indiceJugadorActual = 0;
              cout << "(Comando correcto) El juego se ha inicializado "
                      "correctamente.\n";
            } else {
              juego.jugadores.limpiar();
              juego.tablero.limpiar();
              cout << "(Archivo sin formato) " << args[0]
                   << " no contiene información en el formato esperado.\n";
            }
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
    1. El jugador existe
    2. El jugador tiene tropas para poner
    3. El jugador tiene un territorio para poner tropas
    4. La seleccion es uno de sus territorios

    Funcionamiento: 
    Esta opcion permite anadir una cantidad de unidades a un territorio.
    menciona los territorios del jugador mientras los guarda en un vector, elijes
    donde y cuantas tropas poner en tu territorio, y si te quedan tropas por poner vuelve 
    a preguntar.
    =======================================================================================
    */
    else if (comando == "obtener_unidades") {
      // Reinicia el estado del turno para el jugador
      jugadorHaAtacado = false;
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
        else if (juego.buscarJugadorPorNombre(args[0]) == nullptr) {
          cout << "(Jugador no válido) El jugador " << args[0]
               << " no forma parte de esta partida.\n";
        } else if (juego.obtenerJugadorEnTurno()->nombre != args[0]) {
          cout << "(Jugador fuera de turno) No es el turno del jugador "
               << args[0] << ".\n";
        } else {
          vector<Territorio *> territorio_posible;
          for (int i = 0; i < 42; i++) {
            if (juego.tablero.territorios.obtener(i).colorOcupante ==
                juego.obtenerJugadorEnTurno()->color) {
              territorio_posible.push_back(
                  &juego.tablero.territorios.obtener(i));
            }
          }

          int tropas_a_anadir = calcular_tropas(territorio_posible.size());
          int bono_continentes = juego.tablero.calcularBonoContinentes(juego.obtenerJugadorEnTurno()->color);
          tropas_a_anadir += bono_continentes;
          if (tropas_a_anadir < 3)
            tropas_a_anadir = 3;

          while (tropas_a_anadir > 0) {
            cout << "\nTienes " << tropas_a_anadir
                 << " unidades para ubicar.\n";
            cout << "En cual de tus territorios deseas ubicar unidades?\n";

            int index = 1;
            for (vector<Territorio *>::iterator i = territorio_posible.begin();
                 i != territorio_posible.end(); ++i) {
              cout << index << ". " << (*i)->nombre
                   << " (Actuales: " << (*i)->numEjercitos << ")\n";
              index++;
            }

            int opt = 0;
            cout << "Elige un territorio (1-" << territorio_posible.size()
                 << "): ";
            cin >> opt;

            if (opt > 0 && opt <= (int)territorio_posible.size()) {
              int cantidad = 0;
              cout << "Cuantas unidades deseas ubicar aqui? (Maximo "
                   << tropas_a_anadir << "): ";
              cin >> cantidad;

              if (cantidad > 0 && cantidad <= tropas_a_anadir) {
                territorio_posible[opt - 1]->modificarEjercitos(cantidad);
                tropas_a_anadir -= cantidad;

                cout << "-> Se han anadido " << cantidad << " tropas a "
                     << (*territorio_posible[opt - 1]).nombre << ".\n";
              } else {
                cout << "(Cantidad invalida, intenta de nuevo)\n";
              }
            } else {
              cout << "(Opcion invalida, intenta de nuevo)\n";
              cin.clear();
              cin.ignore(10000, '\n');
            }
          }

          // Limpiar el buffer de cin por si quedó un salto de línea antes de
          cin.ignore(10000, '\n');

          Jugador *jugadorActual = juego.obtenerJugadorEnTurno();
          jugadorActual->faseActual = FASE_ATACAR; // Avanza a la siguiente fase
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
    1. Filtra los territorios disponibles, confirmando que solamente se den resultados validos
       (Mas de 1 tropa y tenga vecinos enemigos). Si no hay territorios válidos, informa de 
       inmediato y finaliza la acción.
    3. Selección de origen: Muestra los territorios válidos y pide al jugador seleccionar 
       desde cuál atacar, junto con la cantidad de tropas a utilizar (las cuales son 
       temporalmente restadas del territorio de origen).
    4. Filtrado de objetivo: Una vez elegido el origen, crea un vector únicamente con los 
       territorios vecinos que pertenecen a otros jugadores, asegurando una selección válida.
    5. Ejecución del ataque: Llama a la función 'atacar()' pasándole las tropas atacantes 
       y defensoras. Debido a que la función recibe estos parámetros por referencia (int &), 
       las bajas se aplican y actualizan directamente en las variables suministradas.
    6. Resultados: Si el ataque es exitoso, se asigna el territorio al atacante y se 
       ubican las tropas sobrevivientes allí. Si fracasa, las tropas sobrevivientes 
       regresan al territorio de origen.
    ===================================================================================
    */
    else if (comando == "atacar") {
      if (args.size() == 1) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.estaTerminado()) {
          cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n";
        } else if (juego.buscarJugadorPorNombre(args[0]) == nullptr) {
          cout << "(Jugador no válido) El jugador " << args[0]
               << " no forma parte de esta partida.\n";
        } else if (juego.obtenerJugadorEnTurno()->nombre != args[0]) {
          cout << "(Jugador fuera de turno) No es el turno del jugador "
               << args[0] << ".\n";
        } else if (juego.obtenerJugadorEnTurno()->faseActual ==
                   FASE_OBTENER_UNIDADES) {
          // Valida que el jugador ya haya obtenido sus unidades
          cout << "(Jugador no ha ubicado unidades) El jugador " << args[0]
               << " no ha ejecutado el comando obtener_unidades.\n";
        } else if (juego.obtenerJugadorEnTurno()->faseActual ==
                   FASE_FORTIFICAR) {
          // Valida que el jugador ya haya fortificado
          cout << "(Jugador ya fortifico) El jugador " << args[0]
               << " ya no puede atacar.\n";
        } else {
          Jugador *jugadorActual = juego.obtenerJugadorEnTurno();

          // Filtrar los territorios del jugador que tienen > 1 tropa Y al menos un vecino enemigo
          vector<Territorio *> misTerritorios;
          for (int i = 0; i < 42; i++) {
            Territorio &ter = juego.tablero.territorios.obtener(i);
            if (ter.colorOcupante == jugadorActual->color && ter.numEjercitos > 1) {
              bool tieneVecinoEnemigo = false;
              for (int j = 0; j < ter.vecinos.longitud(); ++j) {
                if (ter.vecinos.obtener(j)->colorOcupante != jugadorActual->color) {
                  tieneVecinoEnemigo = true;
                  break;
                }
              }
              if (tieneVecinoEnemigo) {
                misTerritorios.push_back(&ter);
              }
            }
          }

          if (misTerritorios.empty()) {
            cout << "(Jugador no puede atacar) " << args[0]
                 << " no tiene territorios con mas de 1 tropa y vecinos enemigos para atacar.\n";
          } else {
            cout << "\nDesde cual de tus territorios quieres atacar? (Ingresa 0 para no atacar / terminar fase de ataque)\n";
            for (size_t i = 0; i < misTerritorios.size(); ++i) {
              cout << i + 1 << ". " << misTerritorios[i]->nombre << " (tienes "
                   << misTerritorios[i]->numEjercitos << " tropas)\n";
            }
            cout << "0. Terminar fase de ataque y pasar a fortificar\n";

            int opcion = -1;
            cin >> opcion;
            if (opcion == 0) {
              jugadorHaAtacado = true;
              jugadorActual->faseActual = FASE_FORTIFICAR;
              cin.ignore(10000, '\n');
              cout << "(Comando correcto) El jugador " << args[0]
                   << " ha terminado de atacar.\n";
            } else if (opcion < 0 || opcion > (int)misTerritorios.size()) {
              cout << "Error: Opcion invalida\n";
            } else {
              Territorio *territorioOrigen = misTerritorios[opcion - 1];
              int maxTropas = territorioOrigen->numEjercitos - 1;

              cout << "Con cuantas tropas quieres atacar? (1 a " << maxTropas << "): \n";
              int tropas = 0;
              cin >> tropas;
              if (tropas <= 0 || tropas > maxTropas) {
                cout << "Error: Opcion invalida\n";
              } else {
                territorioOrigen->modificarEjercitos(-tropas);

                // Filtrar únicamente los vecinos que pertenecen a otros jugadores
                vector<Territorio *> territoriosAtacar;
                for (int i = 0; i < territorioOrigen->vecinos.longitud(); ++i) {
                  Territorio *vecino = territorioOrigen->vecinos.obtener(i);
                  if (vecino->colorOcupante != jugadorActual->color) {
                    territoriosAtacar.push_back(vecino);
                  }
                }

                cout << "\nA cual territorio quieres atacar?\n";
                for (size_t i = 0; i < territoriosAtacar.size(); ++i) {
                  cout << i + 1 << ". " << territoriosAtacar[i]->nombre
                       << " (Propietario: " << territoriosAtacar[i]->colorOcupante
                       << " - Tropas: " << territoriosAtacar[i]->numEjercitos << ")\n";
                }

                int opcion2 = 0;
                do {
                  cin >> opcion2;
                  if (opcion2 <= 0 || opcion2 > (int)territoriosAtacar.size()) {
                    cout << "Error: Opcion invalida\n";
                  }
                } while (opcion2 <= 0 || opcion2 > (int)territoriosAtacar.size());

                Territorio *territorioDefensor = territoriosAtacar[opcion2 - 1];
                bool gano = atacar(tropas, territorioDefensor->numEjercitos);
                jugadorHaAtacado = true;

                if (gano) {
                  territorioDefensor->asignarOcupante(jugadorActual->color, tropas);
                  cout << "¡Has conquistado el territorio " << territorioDefensor->nombre << "!\n";
                  cout << "Tropas ubicadas en ese territorio: " << tropas << "\n";
                } else {
                  territorioOrigen->modificarEjercitos(tropas);
                  cout << "Ataque finalizado. Tropas sobrevivientes devueltas a tu territorio.\n";
                }
              }
              cin.ignore(10000, '\n');
              cout << "(Comando correcto) El jugador " << args[0]
                   << " ha terminado de atacar.\n";
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
    3. Muestra una lista de territorios propios que tienen al menos un vecino propio y >1 tropa.
    4. Pide seleccionar un territorio de origen y luego un vecino propio como destino.
    5. Solicita la cantidad de tropas a mover (respetando que debe quedar al menos 1).
    6. Realiza el movimiento y pasa el turno al siguiente jugador.
    ===================================================================================
    */
    else if (comando == "fortificar") {
      if (args.size() == 1) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.estaTerminado()) {
          cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n";
        } else if (juego.buscarJugadorPorNombre(args[0]) == nullptr) {
          cout << "(Jugador no válido) El jugador " << args[0]
               << " no forma parte de esta partida.\n";
        } else if (juego.obtenerJugadorEnTurno()->nombre != args[0]) {
          cout << "(Jugador fuera de turno) No es el turno del jugador "
               << args[0] << ".\n";
        } else if (!jugadorHaAtacado) {
          // Valida que el jugador haya pasado por la fase de ataque primero
          cout << "(Jugador no ha atacado) El jugador " << args[0]
               << " no ha ejecutado el comando atacar.\n";
        } else {
          Jugador *jugadorActual = juego.obtenerJugadorEnTurno();
          jugadorActual->faseActual = FASE_FORTIFICAR;
          vector<Territorio *> territoriosDisponibles;
          vector<Territorio *> vecinosDisponibles;
          Territorio *a;
          Territorio *b;
          string eleccion;
          int opcion = 0;

          cout << "¿Quieres fortificar algun territorio? (s/n): ";
          cin >> eleccion;
          if (eleccion == "s" || eleccion == "S") {
            for (int i = 0; i < 42; i++) {
              Territorio &t = juego.tablero.territorios.obtener(i);
              if (t.colorOcupante == jugadorActual->color && t.numEjercitos > 1) {
                bool tieneVecinoPropio = false;
                for (int j = 0; j < t.vecinos.longitud(); j++) {
                  if (t.vecinos.obtener(j)->colorOcupante == jugadorActual->color) {
                    tieneVecinoPropio = true;
                    break;
                  }
                }
                if (tieneVecinoPropio) {
                  territoriosDisponibles.push_back(&t);
                }
              }
            }

            if (territoriosDisponibles.empty()) {
              cout << "No tienes territorios con tropas suficientes y vecinos propios para fortificar.\n";
            } else {
              cout << "\nDesde que territorio quieres mover unidades?\n";
              for (size_t i = 0; i < territoriosDisponibles.size(); i++) {
                cout << i + 1 << ". " << territoriosDisponibles[i]->nombre 
                     << " (" << territoriosDisponibles[i]->numEjercitos << " tropas)\n";
              }

              while (opcion <= 0 || opcion > (int)territoriosDisponibles.size()) {
                cin >> opcion;
                if (opcion <= 0 || opcion > (int)territoriosDisponibles.size()) {
                  cout << "Error: Opcion invalida. Intenta de nuevo: ";
                }
              }

              a = territoriosDisponibles[opcion - 1];

              cout << "\nHacia que vecino quieres fortificar?\n";
              for (int i = 0; i < a->vecinos.longitud(); i++) {
                if (a->vecinos.obtener(i)->colorOcupante == jugadorActual->color) {
                  vecinosDisponibles.push_back(a->vecinos.obtener(i));
                }
              }

              for (size_t i = 0; i < vecinosDisponibles.size(); i++) {
                cout << i + 1 << ". " << vecinosDisponibles[i]->nombre 
                     << " (" << vecinosDisponibles[i]->numEjercitos << " tropas)\n";
              }

              opcion = 0;
              while (opcion <= 0 || opcion > (int)vecinosDisponibles.size()) {
                cin >> opcion;
                if (opcion <= 0 || opcion > (int)vecinosDisponibles.size()) {
                  cout << "Error: Opcion invalida. Intenta de nuevo: ";
                }
              }

              b = vecinosDisponibles[opcion - 1];

              int maxTropas = a->numEjercitos - 1;
              int num = 0;
              cout << "¿Cuantas tropas deseas mover? (1 a " << maxTropas << "): ";
              while (num <= 0 || num > maxTropas) {
                cin >> num;
                if (num <= 0 || num > maxTropas) {
                  cout << "Error: Opcion invalida. Intenta de nuevo: ";
                }
              }

              a->modificarEjercitos(-num);
              b->modificarEjercitos(num);

              cout << "Has movido " << num << " tropas de " << a->nombre << " a " << b->nombre << ".\n";
            }
          }
          
          cin.clear();
          cin.ignore(10000, '\n');
          cout << "(Comando correcto) El jugador " << args[0]
               << " ha terminado de fortificar su posición.\n";
          // Al terminar la fortificación, finaliza el turno del jugador actual
          juego.pasarAlSiguienteTurno();
          jugadorHaAtacado = false;
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
          cout << "Jugadores en partida (" << juego.jugadores.longitud()
               << "):\n";
          for (int i = 0; i < juego.jugadores.longitud(); ++i) {
            Jugador &jug = juego.jugadores.obtener(i);
            cout << " - " << jug.nombre << " [Color: " << jug.color << "]\n";
          }

          Jugador *enTurno = juego.obtenerJugadorEnTurno();
          if (enTurno != nullptr) {
            cout << "\nJugador con el turno actual: " << enTurno->nombre
                 << "\n\n";
          }

          cout << "Lista de Territorios:\n";
          for (int i = 0; i < juego.tablero.territorios.longitud(); ++i) {
            Territorio &ter = juego.tablero.territorios.obtener(i);
            cout << " - " << ter.codigo << " (" << ter.nombre << "): "
                 << "Controlado por '" << ter.colorOcupante << "' con "
                 << ter.numEjercitos << " unidades.\n";
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
    (Para la Entrega 2)
    1. Guarda el estado actual del juego en un archivo de texto con el mismo formato
       utilizado en la inicializacion.
    ===================================================================================
    */
    else if (comando == "guardar") {
      if (args.size() == 1) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else {
          // TODO: Lógica de serialización del estado del juego en archivo de
          // texto
          cout << "(Comando correcto) La partida ha sido guardada "
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
    (Para la Entrega 2)
    1. Guarda el estado actual del juego utilizando el algoritmo de codificacion de 
       Huffman en un archivo binario.
    ===================================================================================
    */
    else if (comando == "guardar_comprimido") {
      if (args.size() == 1) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else {
          // TODO: Lógica de compresión mediante Huffman (Componente 2)
          cout << "(Comando correcto) La partida ha sido codificada y guardada "
                  "correctamente.\n";
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
